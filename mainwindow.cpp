#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QJsonDocument>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>

#include "telemetrywindow.h"
#include "serialscanner.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _mavlinkContext(new MavlinkContext())
{
    ui->setupUi(this);
    _telemetry = new TelemetryWindow(nullptr);
    _parameterList.setWindowTitle("Parameters list");
    _logsWindow.setMavlinkContext(_mavlinkContext);
    _mavlinkContext->moveToThread(&_mavlinkThread);
    connect(&_mavlinkThread, &QThread::started, _mavlinkContext, &MavlinkContext::loadModes);
    connect(_mavlinkContext, &MavlinkContext::modeUpdated, ui->modeValue, &QLabel::setText);
    connect(_mavlinkContext, &MavlinkContext::armedUpdated, ui->armedStatus, &QLabel::setText);
    connect(_mavlinkContext, &MavlinkContext::attitudeUpdated, ui->attitude, &QLabel::setText);
    connect(_mavlinkContext, &MavlinkContext::speedsUpdated, ui->speeds, &QLabel::setText);
    connect(_mavlinkContext, &MavlinkContext::altitudeUpdated, ui->altitude, &QLabel::setText);
    connect(_mavlinkContext, &MavlinkContext::logUpdated, this, [this](QString msg, QString severity, QColor color) {
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
    connect(_mavlinkContext, &MavlinkContext::logEntryRecieved, this, [this](const mavlink_log_entry_t& logEntry){
        _logsWindow.handleMavlink(logEntry);
    });
    connect(_mavlinkContext, &MavlinkContext::logDataRecieved, this, [this](const mavlink_log_data_t& logData, const mavlink_message_t& msg){
        _logsWindow.handleMavlink(logData, msg);
    });

    connect(&_parameterList, &ParameterList::setParameterRequest, this, &MainWindow::setParamRequested);
    connect(this, &MainWindow::setParamRequest, _mavlinkContext, &MavlinkContext::sendCommand);

    connect(_mavlinkContext, &MavlinkContext::globalPositionIntUpdated, _telemetry, &TelemetryWindow::onGlobalPositionIntUpdated);

    connect(&_mavlinkThread, &QThread::finished, _mavlinkContext, &QObject::deleteLater);
    _mavlinkThread.start(QThread::LowPriority);

    connect(ui->serialScanner, &SerialScanner::connectSerialDevice, _mavlinkContext, &MavlinkContext::onConnectSerialDevice);

    connect(_mavlinkContext, &MavlinkContext::deviceConnected, ui->devicesTable, &DevicesTable::onDeviceConnected);
    connect(_mavlinkContext, &MavlinkContext::deviceDisconnected, ui->devicesTable, &DevicesTable::onDeviceDisconnected);
    connect(_mavlinkContext, &MavlinkContext::deviceStateChanged, ui->devicesTable, &DevicesTable::onDeviceStateChanged);
}

MainWindow::~MainWindow()
{
    _mavlinkThread.quit();
    _mavlinkThread.wait();
    _telemetry->deleteLater();
    delete ui;
}

void MainWindow::on_actionParameters_set_triggered()
{
    if(_parameterList.isHidden())
        _parameterList.show();
}


void MainWindow::on_actionRefresh_configs_triggered()
{
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
    if (_telemetry->isHidden()) {
        _telemetry->show();
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
