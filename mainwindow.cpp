#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QJsonDocument>
#include <QFile>

MavlinkContext::MavlinkContext()
    : _localDefaultDevice(14550, "0.0.0.0", this)
    , _heartBeatTimer(this)
{
    _heartBeatTimer.start(1000);
    connect(&_heartBeatTimer, &QTimer::timeout, this, [this](){
        //if (_localDefaultDevice.getSocketThread() != );
    });

    connect(&_attitude, &Attitude::orientationUpdated, this, [this](const QVector3D& orientation) {
        QString result("R: %1°, P: %2°, Y: %3°");
        emit attitudeUpdated(result
                                    .arg(QString::asprintf("%.2f", orientation.x()))
                                    .arg(QString::asprintf("%.2f", orientation.y()))
                                    .arg(QString::asprintf("%.2f", orientation.z())));
    });

    connect(&_lPositionNED, &LocalPositionNED::speedsUpdated, this, [this](const QVector3D speeds) {
        QString result("FS: %1 m/s, LS: %2 m/s, VS: %3 m/s");
        emit speedsUpdated(result
                                .arg(QString::asprintf("%.2f", speeds.x()))
                                .arg(QString::asprintf("%.2f", speeds.y()))
                                .arg(QString::asprintf("%.2f", speeds.z()))
        );
    });

    connect(&_statusText, &StatusText::logUpdated, this, [this](QString msg, QString severity) {
        QColor resultColor;
        if(severity == "EMERGENCY") {
            resultColor = Qt::red;
        }
        else if(severity == "ALERT") {
            resultColor = Qt::red;
        }
        else if(severity == "CRITICAL") {
            resultColor = Qt::red;
        }
        else if (severity == "ERROR") {
            resultColor = Qt::red;
        }
        else if (severity == "WARNING") {
            resultColor = Qt::yellow;
        }
        else if (severity == "NOTICE") {
            resultColor = Qt::green;
        }
        else if (severity == "INFO") {
            resultColor = Qt::black;
        }
        else if (severity == "DEBUG") {
            resultColor = Qt::blue;
        }
        emit logUpdated(msg, severity, resultColor);
    });

    connect(&_localDefaultDevice, &UDPMavlinkDevice::messageReceived, this, &MavlinkContext::handleMavlinkMessage);
}

void MavlinkContext::sendHeartbeat() {
    mavlink_msg_heartbeat_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &_heartBeatMsg,
        MAV_TYPE_GCS,
        MAV_AUTOPILOT_INVALID,
        0,
        0,
        MAV_STATE_ACTIVE
    );
    _localDefaultDevice.sendCommand(_heartBeatMsg);
}

void MavlinkContext::updateMode(uint8_t autopilot, uint8_t type, uint32_t customMode) {
    switch(autopilot) {
        case MAV_AUTOPILOT_ARDUPILOTMEGA:
        QString stringifiedCustomMode = QString::number(customMode);
            if(_existingModes.contains(stringifiedCustomMode))
                emit modeUpdated(_existingModes[stringifiedCustomMode].toString());
            else
                emit modeUpdated("UNKNOWN");
        break;
    }
}

void MavlinkContext::handleMavlinkMessage(mavlink_message_t msg) {
    switch(msg.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(&msg, &heartbeat);
        heartbeatMessageReceived(msg);
        heartbeatUpdated(heartbeat);
        updateMode(heartbeat.autopilot, heartbeat.type, heartbeat.custom_mode);
        armedUpdated((heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) ? "ARMED" : "DISARMED");
    break;
    case MAVLINK_MSG_ID_ATTITUDE:
        mavlink_attitude_t attitude;
        mavlink_msg_attitude_decode(&msg, &attitude);
        _attitude.handleMavlink(attitude);
    break;
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
        mavlink_local_position_ned_t localPositionNED;
        mavlink_msg_local_position_ned_decode(&msg, &localPositionNED);
        _lPositionNED.handleMavlink(localPositionNED);
    break;
    case MAVLINK_MSG_ID_STATUSTEXT:
        mavlink_statustext_t statusText;
        mavlink_msg_statustext_decode(&msg, &statusText);
        _statusText.handleMavlink(statusText);
    break;
    case MAVLINK_MSG_ID_PARAM_VALUE:
        mavlink_param_value_t paramValue;
        mavlink_msg_param_value_decode(&msg, &paramValue);
        emit paramUpdated(paramValue);
    break;
    case MAVLINK_MSG_ID_PARAM_EXT_VALUE:
        mavlink_param_ext_value_t paramExtValue;
        mavlink_msg_param_ext_value_decode(&msg, &paramExtValue);
        emit paramExtUpdated(paramExtValue);
    break;
    }
}

void MavlinkContext::loadModes() {
    QFile file("modes.json");
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open file: " << "modes.json";
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if(error.error != QJsonParseError::NoError) {
        qWarning() << "modes.json parse error: " << error.errorString();
        return;
    }

    _existingModes = doc.object();
}

void MavlinkContext::sendCommand(const mavlink_message_t& msg) {
    _localDefaultDevice.sendCommand(msg);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _mavlinkContext.moveToThread(&_mavlinkThread);
    connect(&_mavlinkThread, &QThread::started, &_mavlinkContext, &MavlinkContext::loadModes);
    connect(&_mavlinkContext, &MavlinkContext::modeUpdated, ui->modeValue, &QLabel::setText);
    connect(&_mavlinkContext, &MavlinkContext::armedUpdated, ui->armedStatus, &QLabel::setText);
    connect(&_mavlinkContext, &MavlinkContext::attitudeUpdated, ui->attitude, &QLabel::setText);
    connect(&_mavlinkContext, &MavlinkContext::speedsUpdated, ui->speeds, &QLabel::setText);
    connect(&_mavlinkContext, &MavlinkContext::logUpdated, this, [this](QString msg, QString severity, QColor color) {
        QListWidgetItem* item = new QListWidgetItem(QString("[%1] %2").arg(severity).arg(msg));
        item->setForeground(color);
        ui->gcsLog->addItem(item);
        ui->gcsLog->scrollToItem(item);
    });
    connect(ui->logAutoScroll, &QCheckBox::toggled, ui->gcsLog, [this](bool checked) {
        ui->gcsLog->setAutoScroll(checked);
    });
    //connect(ui->gcsLog->model(), &QAbstractItemModel::rowsInserted, [this](const QModelIndex& parent, int first, int last) {
    //    ui->gcsLog->setAutoScroll(ui->logAutoScroll->isChecked());
    //});

    connect(&_mavlinkContext, &MavlinkContext::heartbeatMessageReceived, &_parameterList, &ParameterList::onAutopilotHeartbeat);

    connect(&_parameterList, &ParameterList::parametersRequest, this, &MainWindow::paramsRequested);
    connect(this, &MainWindow::paramsRequest, &_mavlinkContext, &MavlinkContext::sendCommand);
    connect(&_mavlinkContext, &MavlinkContext::paramUpdated, this, [this](const mavlink_param_value_t& param){
        _parameterList.handleMavlink(param);
    });
    connect(&_mavlinkContext, &MavlinkContext::paramExtUpdated, this, [this](const mavlink_param_ext_value_t& param){
        _parameterList.handleMavlink(param);
    });

    connect(&_parameterList, &ParameterList::setParameterRequest, this, &MainWindow::setParamRequested);
    connect(this, &MainWindow::setParamRequest, &_mavlinkContext, &MavlinkContext::sendCommand);

    _mavlinkThread.start(QThread::LowPriority);
}

MainWindow::~MainWindow()
{
    _mavlinkThread.quit();
    _mavlinkThread.wait();
    delete ui;
}

void MainWindow::on_actionParameters_set_triggered()
{
    if(_parameterList.isHidden())
        _parameterList.show();
}


void MainWindow::on_actionRefresh_configs_triggered()
{
    _mavlinkContext.loadModes();
}

void MainWindow::onParamUpdated(const mavlink_param_value_t& param) {
    _parameterList.handleMavlink(param);
}

void MainWindow::onParamExtUpdated(const mavlink_param_value_t& param) {
    _parameterList.handleMavlink(param);
}

void MainWindow::paramsRequested(const mavlink_message_t& msg) {
    emit paramsRequest(msg);
}

void MainWindow::setParamRequested(const mavlink_message_t& msg) {
    emit setParamRequest(msg);
}
