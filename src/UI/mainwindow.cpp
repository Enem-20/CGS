#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QJsonDocument>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>
#include <QMetaObject>

#include "telemetry/telemetrywindow.h"
#include "deviceManagement/serialscanner.h"
#include "UI/logs/logplotwindow.h"

#include "vehicle/Vehicle.h"
#include "vehicle/logs/vehiclelogs.h"
#include "vehicle/telemetry/vehicletelemetry.h"
#include "vehicle/statuslog/vehiclestatuslog.h"
#include "vehicle/parameters/Parameters.h"

#include "widgets/attitudelabel.h"
#include "widgets/altitudelabel.h"
#include "widgets/speedslabel.h"
#include "widgets/gcslog.h"

#include "deviceManagement/basedevice.h"
#include "deviceManagement/serial/serialdevice.h"
#include "deviceManagement/DeviceManager.h"
#include "protocols/ProtocolMonitor.h"
#include "channelManagement/ChannelManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ui->devicesContainer->layout()->addWidget(&_defaultDeviceWidget);

    connect(ui->logAutoScroll, &QCheckBox::toggled, ui->gcsLog, [this](bool checked) {
        ui->gcsLog->setAutoScroll(checked);
    });
    //connect(ui->gcsLog->model(), &QAbstractItemModel::rowsInserted, [this](const QModelIndex& parent, int first, int last) {
    //    ui->gcsLog->setAutoScroll(ui->logAutoScroll->isChecked());
    //});

    // connect serial scanner


    connect(DeviceManager::getInstance(), &DeviceManager::deviceCreated, this, &MainWindow::onDeviceCreated);
    connect(ChannelManager::getInstance(), &ChannelManager::activeChannelChanged, this, &MainWindow::onActiveChannelChanged);

    DeviceManager::getInstance()->createDefaultDevice();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionParameters_set_triggered() {
    if (_parameterList.isHidden()) {
        _parameterList.show();
    }
}

void MainWindow::on_actionRefresh_configs_triggered() {
    // ui->modeValue->loadModes();
}

void MainWindow::on_actionOpen_Telemetry_triggered() {
    if (_telemetryWindow.isHidden()) {
        _telemetryWindow.show();
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

void MainWindow::onDeviceCreated(uint64_t id) {
    BaseDevice* device = DeviceManager::getInstance()->getDevice(id);
    if (device) {
        ChannelManager* channelManager = ChannelManager::getInstance();
        connect(device->getProtocolMonitor(), &ProtocolMonitor::channelDetected, channelManager, &ChannelManager::onChannelDetected);
        connect(device->getProtocolMonitor(), &ProtocolMonitor::channelLost, channelManager, &ChannelManager::onChannelLost);
    }
}

void MainWindow::onActiveChannelChanged(RemoteChannelID id) {
    // if (_activeVehicle) {
    //     // disconnect VehicleLogs
    //     disconnect(_activeVehicle->getLogs(), &VehicleLogs::logsListReceived, &_logsWindow, &LogsWindow::onLogListReceived);
    //     disconnect(_activeVehicle->getLogs(), &VehicleLogs::logDownloadProgressUpdated, &_logsWindow, &LogsWindow::onLogDownloadProgressUpdated);
    //     disconnect(_activeVehicle->getLogs(), &VehicleLogs::logFileDownloaded, &_logsWindow, &LogsWindow::onLogFileDownloaded);
    //     disconnect(_activeVehicle->getLogs(), &VehicleLogs::stateChanged, &_logsWindow, &LogsWindow::onLogSystemStateChanged);

    //     // disconnect VehicleTelemetry
    //     disconnect(_activeVehicle->getTelemetry(), &VehicleTelemetry::orientationUpdated, &_telemetryWindow, &TelemetryWindow::onVehicleOrientationUpdated);
    //     disconnect(_activeVehicle->getTelemetry(), &VehicleTelemetry::velocityUpdated, &_telemetryWindow, &TelemetryWindow::onVehicleVelocityUpdated);
    //     disconnect(_activeVehicle->getTelemetry(), &VehicleTelemetry::altitudeUpdated, &_telemetryWindow, &TelemetryWindow::onVehicleAltitudeUpdated);
    //     disconnect(_activeVehicle->getTelemetry(), &VehicleTelemetry::orientationUpdated, ui->attitude, &AttitudeLabel::onOrientationUpdated);
    //     disconnect(_activeVehicle->getTelemetry(), &VehicleTelemetry::altitudeUpdated, ui->altitude, &AltitudeLabel::onAltitudeUpdated);
    //     disconnect(_activeVehicle->getTelemetry(), &VehicleTelemetry::velocityUpdated, ui->speeds, &SpeedsLabel::onVelocityUpdated);
    //     // disconnect(_activeVehicle->getTelemetry(), &VehicleTelemetry::modeUpdated, ui->modeValue, &ModeLabel::onModeUpdated);
    //     // disconnect(_activeVehicle->getTelemetry(), &VehicleTelemetry::armStateUpdated, ui->armedStatus, &ArmLabel::onArmStateUpdated);

    //     // disconnect VehicleStatusLog
    //     disconnect(_activeVehicle->getStatusLog(), &VehicleStatusLog::statusMessageReceived, &_telemetryWindow, &TelemetryWindow::onStatusMessageReceived);
    //     disconnect(_activeVehicle->getStatusLog(), &VehicleStatusLog::statusMessageReceived, ui->gcsLog, &GCSLog::onLogUpdated);

    //     // disconnect Parameters
    //     disconnect(_activeVehicle->getParameters(), &Parameters::newParameterReceived, &_parameterList, &ParameterList::onParameterUpdate);
    //     disconnect(_activeVehicle->getParameters(), &Parameters::parameterUpdated, &_parameterList, &ParameterList::onParameterUpdate);

    //     // disconnect LogsWindow
    //     disconnect(&_logsWindow, &LogsWindow::requestLogList, _activeVehicle->getLogs(), &VehicleLogs::onLogsListRequested);
    //     disconnect(&_logsWindow, &LogsWindow::requestLogDownload, _activeVehicle->getLogs(), &VehicleLogs::onLogDownloadRequested);
    //     disconnect(&_logsWindow, &LogsWindow::requestLogDownloadStop, _activeVehicle->getLogs(), &VehicleLogs::onLogDownloadStopRequested);
    //     disconnect(&_logsWindow, &LogsWindow::requestLogErase, _activeVehicle->getLogs(), &VehicleLogs::onEraseLogsRequested);

    //     // disconnect ParameterList
    //     disconnect(&_parameterList, &ParameterList::parameterSetRequest, _activeVehicle->getParameters(), &Parameters::onSendSingleToVehicle);
    //     disconnect(&_parameterList, &ParameterList::parametersSetRequest, _activeVehicle->getParameters(), &Parameters::onSendToVehicle);
    //     disconnect(&_parameterList, &ParameterList::parameterReadRequest, _activeVehicle->getParameters(), &Parameters::onRequestSingleFromVehicle);
    //     disconnect(&_parameterList, &ParameterList::parametersReadRequest, _activeVehicle->getParameters(), &Parameters::onRequestAllFromVehicle);

    //     _activeVehicle = nullptr;
    // }

    // _activeVehicle = ChannelManager::getInstance()->getVehicle(id);

    // if (_activeVehicle) {
    //     // connect VehicleLogs
    //     connect(_activeVehicle->getLogs(), &VehicleLogs::logsListReceived, &_logsWindow, &LogsWindow::onLogListReceived);
    //     connect(_activeVehicle->getLogs(), &VehicleLogs::logDownloadProgressUpdated, &_logsWindow, &LogsWindow::onLogDownloadProgressUpdated);
    //     connect(_activeVehicle->getLogs(), &VehicleLogs::logFileDownloaded, &_logsWindow, &LogsWindow::onLogFileDownloaded);
    //     connect(_activeVehicle->getLogs(), &VehicleLogs::stateChanged, &_logsWindow, &LogsWindow::onLogSystemStateChanged);

    //     // connect VehicleTelemetry
    //     connect(_activeVehicle->getTelemetry(), &VehicleTelemetry::orientationUpdated, &_telemetryWindow, &TelemetryWindow::onVehicleOrientationUpdated);
    //     connect(_activeVehicle->getTelemetry(), &VehicleTelemetry::velocityUpdated, &_telemetryWindow, &TelemetryWindow::onVehicleVelocityUpdated);
    //     connect(_activeVehicle->getTelemetry(), &VehicleTelemetry::altitudeUpdated, &_telemetryWindow, &TelemetryWindow::onVehicleAltitudeUpdated);
    //     connect(_activeVehicle->getTelemetry(), &VehicleTelemetry::orientationUpdated, ui->attitude, &AttitudeLabel::onOrientationUpdated);
    //     connect(_activeVehicle->getTelemetry(), &VehicleTelemetry::altitudeUpdated, ui->altitude, &AltitudeLabel::onAltitudeUpdated);
    //     connect(_activeVehicle->getTelemetry(), &VehicleTelemetry::velocityUpdated, ui->speeds, &SpeedsLabel::onVelocityUpdated);
    //     // connect(_activeVehicle->getTelemetry(), &VehicleTelemetry::modeUpdated, ui->modeValue, &ModeLabel::onModeUpdated);
    //     // connect(_activeVehicle->getTelemetry(), &VehicleTelemetry::armStateUpdated, ui->armedStatus, &ArmLabel::onArmStateUpdated);

    //     // connect VehicleStatusLog
    //     connect(_activeVehicle->getStatusLog(), &VehicleStatusLog::statusMessageReceived, &_telemetryWindow, &TelemetryWindow::onStatusMessageReceived);
    //     connect(_activeVehicle->getStatusLog(), &VehicleStatusLog::statusMessageReceived, ui->gcsLog, &GCSLog::onLogUpdated);

    //     // connect Parameters
    //     connect(_activeVehicle->getParameters(), &Parameters::newParameterReceived, &_parameterList, &ParameterList::onParameterUpdate);
    //     connect(_activeVehicle->getParameters(), &Parameters::parameterUpdated, &_parameterList, &ParameterList::onParameterUpdate);

    //     // connect LogsWindow
    //     connect(&_logsWindow, &LogsWindow::requestLogList, _activeVehicle->getLogs(), &VehicleLogs::onLogsListRequested);
    //     connect(&_logsWindow, &LogsWindow::requestLogDownload, _activeVehicle->getLogs(), &VehicleLogs::onLogDownloadRequested);
    //     connect(&_logsWindow, &LogsWindow::requestLogDownloadStop, _activeVehicle->getLogs(), &VehicleLogs::onLogDownloadStopRequested);
    //     connect(&_logsWindow, &LogsWindow::requestLogErase, _activeVehicle->getLogs(), &VehicleLogs::onEraseLogsRequested);

    //     // connect ParameterList
    //     connect(&_parameterList, &ParameterList::parameterSetRequest, _activeVehicle->getParameters(), &Parameters::onSendSingleToVehicle);
    //     connect(&_parameterList, &ParameterList::parametersSetRequest, _activeVehicle->getParameters(), &Parameters::onSendToVehicle);
    //     connect(&_parameterList, &ParameterList::parameterReadRequest, _activeVehicle->getParameters(), &Parameters::onRequestSingleFromVehicle);
    //     connect(&_parameterList, &ParameterList::parametersReadRequest, _activeVehicle->getParameters(), &Parameters::onRequestAllFromVehicle);
    // }
}
