#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QJsonDocument>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>

#include "telemetry/telemetrywindow.h"
#include "deviceManagement/serialscanner.h"
#include "UI/logs/logplotwindow.h"

#include "attitudelabel.h"
#include "altitudelabel.h"
#include "speedslabel.h"
#include "gcslog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _mavlinkContext(new MavlinkContext())
{
    ui->setupUi(this);

    _mavlinkContext->moveToThread(&_mavlinkThread);

    _mavlinkContext->subscribe(&_vehicleTelemetry);
    connect(&_vehicleTelemetry, &VehicleTelemetry::orientationUpdated, ui->attitude, &AttitudeLabel::onOrientationUpdated);
    connect(&_vehicleTelemetry, &VehicleTelemetry::altitudeUpdated, ui->altitude, &AltitudeLabel::onAltitudeUpdated);
    connect(&_vehicleTelemetry, &VehicleTelemetry::speedsUpdated, ui->speeds, &SpeedsLabel::onSpeedsUpdated);
    // connect(&_vehicleTelemetry, &VehicleTelemetry::, _telemetry, &TelemetryWindow::onGlobalPositionIntUpdated);

    _mavlinkContext->subscribe(&_statusText);
    connect(&_statusText, &StatusText::logUpdated, ui->gcsLog, &GCSLog::onLogUpdated);

    connect(&_mavlinkThread, &QThread::started, _mavlinkContext, &MavlinkContext::loadModes);
    connect(_mavlinkContext, &MavlinkContext::modeUpdated, ui->modeValue, &QLabel::setText);
    connect(_mavlinkContext, &MavlinkContext::armedUpdated, ui->armedStatus, &QLabel::setText);
    connect(_mavlinkContext, &MavlinkContext::attitudeUpdated, ui->attitude, &QLabel::setText);
    connect(_mavlinkContext, &MavlinkContext::speedsUpdated, ui->speeds, &QLabel::setText);
    connect(_mavlinkContext, &MavlinkContext::altitudeUpdated, ui->altitude, &QLabel::setText);

    connect(ui->logAutoScroll, &QCheckBox::toggled, ui->gcsLog, [this](bool checked) {
        ui->gcsLog->setAutoScroll(checked);
    });
    //connect(ui->gcsLog->model(), &QAbstractItemModel::rowsInserted, [this](const QModelIndex& parent, int first, int last) {
    //    ui->gcsLog->setAutoScroll(ui->logAutoScroll->isChecked());
    //});

    connect(_mavlinkContext, &MavlinkContext::heartbeatMessageReceived, &_parameterList, &ParameterList::onAutopilotHeartbeat);

    connect(&_parameterList, &ParameterList::parametersRequest, this, &MainWindow::paramsRequested);
    connect(&_parameterList, &ParameterList::singleParameterRequest, _mavlinkContext, &MavlinkContext::sendCommand);
    connect(this, &MainWindow::paramsRequest, _mavlinkContext, &MavlinkContext::sendCommand);
    connect(_mavlinkContext, &MavlinkContext::paramUpdated, &_parameterList,
        QOverload<const mavlink_param_value_t&>::of(&ParameterList::handleMavlink)
    );

    connect(_mavlinkContext, &MavlinkContext::paramExtUpdated, &_parameterList,
        QOverload<const mavlink_param_ext_value_t&>::of(&ParameterList::handleMavlink)
    );
    connect(_mavlinkContext, &MavlinkContext::paramAckRecieved, &_parameterList,
        QOverload<const mavlink_param_ext_ack_t&>::of(&ParameterList::handleMavlink)
    );
    connect(_mavlinkContext, &MavlinkContext::logEntryRecieved, &_logsWindow,
        QOverload<const mavlink_log_entry_t&>::of(&LogsWindow::handleMavlink)
    );
    connect(_mavlinkContext, &MavlinkContext::logDataRecieved, &_logsWindow,
        QOverload<const mavlink_log_data_t&>::of(&LogsWindow::handleMavlink)
    );

    connect(&_parameterList, &ParameterList::setParameterRequest, this, &MainWindow::setParamRequested);
    connect(this, &MainWindow::setParamRequest, _mavlinkContext, &MavlinkContext::sendCommand);


    connect(&_mavlinkThread, &QThread::finished, _mavlinkContext, &QObject::deleteLater);
    _mavlinkThread.start(QThread::LowPriority);

    connect(ui->serialScanner, QOverload<QSerialPortInfo>::of(&SerialScanner::connectSerialDevice), _mavlinkContext,
            QOverload<QSerialPortInfo>::of(&MavlinkContext::onConnectSerialDevice)
        );

    connect(ui->serialScanner, QOverload<QSerialPortInfo, int32_t, uint8_t, uint8_t, uint8_t, uint8_t>::of(&SerialScanner::connectSerialDevice), _mavlinkContext,
            QOverload<QSerialPortInfo, int32_t, uint8_t, uint8_t, uint8_t, uint8_t>::of(&MavlinkContext::onConnectSerialDevice)
            );

    connect(_mavlinkContext, &MavlinkContext::deviceConnected, ui->devicesTable, &DevicesTable::onDeviceConnected);
    connect(_mavlinkContext, &MavlinkContext::deviceDisconnected, ui->devicesTable, &DevicesTable::onDeviceDisconnected);
    connect(_mavlinkContext, &MavlinkContext::deviceStateChanged, ui->devicesTable, &DevicesTable::onDeviceStateChanged);

    ui->devicesContainer->layout()->addWidget(&_defaultDeviceWidget);
    connect(&_defaultDeviceWidget, &DefaultDeviceWidget::makeDefaultDeviceActive, _mavlinkContext, &MavlinkContext::onMakeDeviceActive);
    connect(_mavlinkContext, &MavlinkContext::activeDeviceChanged, &_defaultDeviceWidget, &DefaultDeviceWidget::onActiveDeviceChanged);

    connect(ui->devicesTable, &DevicesTable::makeDeviceActive, _mavlinkContext, &MavlinkContext::onMakeDeviceActive);

    connect(_mavlinkContext, &MavlinkContext::activeDeviceChanged, &_logsWindow, &LogsWindow::onActiveDeviceChanged);
    connect(_mavlinkContext, &MavlinkContext::activeDeviceChanged, &_parameterList, &ParameterList::onActiveDeviceChanged);
    connect(&_parameterList, &ParameterList::parametersPullingCompleted, _mavlinkContext, &MavlinkContext::onParameterListDownloadCompleted);

    connect(&_logsWindow, &LogsWindow::sendCommand, _mavlinkContext, &MavlinkContext::sendCommand);
}

MainWindow::~MainWindow() {
    _mavlinkThread.quit();
    _mavlinkThread.wait();
    delete ui;
}

void MainWindow::on_actionParameters_set_triggered() {
    if(_parameterList.isHidden())
        _parameterList.show();
}


void MainWindow::on_actionRefresh_configs_triggered() {
    _mavlinkContext->loadModes();
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

void MainWindow::on_actionOpen_Telemetry_triggered() {
    if (_telemetry.isHidden()) {
        _telemetry.show();
    }
}

void MainWindow::on_actionDownload_Log_triggered() {
    if (_logsWindow.isHidden()) {
        _logsWindow.show();
    }
}

void MainWindow::on_actionReview_Log_triggered() {
    LogPlotWindow::openFileToReview();
}
