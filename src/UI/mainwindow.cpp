#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QJsonDocument>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>

#include "telemetry/telemetrywindow.h"
#include "deviceManagement/serialscanner.h"
#include "UI/logs/logplotwindow.h"

#include "protocols/mavlink/mavlinkpacketizer.h"

#include "vehicle/logs/vehiclelogs.h"
#include "vehicle/telemetry/vehicletelemetry.h"
#include "vehicle/statuslog/vehiclestatuslog.h"
#include "vehicle/parameters/Parameters.h"
#include "vehicle/mavlink/mavlinkvehicle.h"

#include "widgets/attitudelabel.h"
#include "widgets/altitudelabel.h"
#include "widgets/speedslabel.h"
#include "widgets/gcslog.h"

#include "deviceManagement/udp/udpdevice.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    // , _mavlinkContext(new MavlinkContext())
{
    ui->setupUi(this);

    // ui->devicesContainer->layout()->addWidget(&_defaultDeviceWidget);

    // _mavlinkContext->moveToThread(&_mavlinkThread);
    // connect(&_mavlinkThread, &QThread::finished, _mavlinkContext, &QObject::deleteLater);
    // _mavlinkThread.start(QThread::LowPriority);

    connect(ui->logAutoScroll, &QCheckBox::toggled, ui->gcsLog, [this](bool checked) {
        ui->gcsLog->setAutoScroll(checked);
    });
    //connect(ui->gcsLog->model(), &QAbstractItemModel::rowsInserted, [this](const QModelIndex& parent, int first, int last) {
    //    ui->gcsLog->setAutoScroll(ui->logAutoScroll->isChecked());
    //});

    _vehicle = new MavlinkVehicle();
    UDPDevice* device = new UDPDevice("DefaultDevice", 14550, "127.0.0.1", this);
    _vehicle->setDevice(static_cast<BaseDevice*>(device));
    MavlinkPacketizer* packetizer = new MavlinkPacketizer();
    _vehicle->setPacketizer(packetizer);
    device->start();
    connect(device, &BaseDevice::portOpened, _vehicle->getParameters(), &Parameters::onConnect);
    connect(device, &BaseDevice::portClosed, _vehicle->getParameters(), &Parameters::onDisconnect);
    // tconnect packetizer
    connect(packetizer, &BasePacketizer::messageReceived, _vehicle->getLogs(), &VehicleLogs::onMessage);
    connect(packetizer, &BasePacketizer::messageReceived, _vehicle->getTelemetry(), &VehicleTelemetry::onMessage);
    connect(packetizer, &BasePacketizer::messageReceived, _vehicle->getStatusLog(), &VehicleStatusLog::onMessage);
    connect(packetizer, &BasePacketizer::messageReceived, _vehicle->getParameters(), &Parameters::onMessage);

    // connect VehicleLogs
    connect(_vehicle->getLogs(), &VehicleLogs::sendMessageRequest, packetizer, &BasePacketizer::onSendMessageRequest);
    connect(_vehicle->getLogs(), &VehicleLogs::logsListReceived, &_logsWindow, &LogsWindow::onLogListReceived);
    connect(_vehicle->getLogs(), &VehicleLogs::logDownloadProgressUpdated, &_logsWindow, &LogsWindow::onLogDownloadProgressUpdated);
    connect(_vehicle->getLogs(), &VehicleLogs::logFileDownloaded, &_logsWindow, &LogsWindow::onLogFileDownloaded);
    connect(_vehicle->getLogs(), &VehicleLogs::stateChanged, &_logsWindow, &LogsWindow::onLogSystemStateChanged);

    // connect VehicleTelemetry
    connect(_vehicle->getTelemetry(), &VehicleTelemetry::sendMessageRequest, packetizer, &BasePacketizer::onSendMessageRequest);
    connect(_vehicle->getTelemetry(), &VehicleTelemetry::orientationUpdated, &_telemetryWindow, &TelemetryWindow::onVehicleOrientationUpdated);
    connect(_vehicle->getTelemetry(), &VehicleTelemetry::velocityUpdated, &_telemetryWindow, &TelemetryWindow::onVehicleVelocityUpdated);
    connect(_vehicle->getTelemetry(), &VehicleTelemetry::altitudeUpdated, &_telemetryWindow, &TelemetryWindow::onVehicleAltitudeUpdated);
    connect(_vehicle->getTelemetry(), &VehicleTelemetry::orientationUpdated, ui->attitude, &AttitudeLabel::onOrientationUpdated);
    connect(_vehicle->getTelemetry(), &VehicleTelemetry::altitudeUpdated, ui->altitude, &AltitudeLabel::onAltitudeUpdated);
    connect(_vehicle->getTelemetry(), &VehicleTelemetry::velocityUpdated, ui->speeds, &SpeedsLabel::onVelocityUpdated);
    // connect((_vehicle->getTelemetry(), &VehicleTelemetry::modeUpdated, ui->modeValue, &ModeLabel::onModeUpdated);
    // connect((_vehicle->getTelemetry(), &VehicleTelemetry::armStateUpdated, ui->armedStatus, &ArmLabel::onArmStateUpdated);

    // connect VehicleStatusLog
    connect(_vehicle->getStatusLog(), &VehicleStatusLog::sendMessageRequest, packetizer, &BasePacketizer::onSendMessageRequest);
    connect(_vehicle->getStatusLog(), &VehicleStatusLog::statusMessageReceived, &_telemetryWindow, &TelemetryWindow::onStatusMessageReceived);
    connect(_vehicle->getStatusLog(), &VehicleStatusLog::statusMessageReceived, ui->gcsLog, &GCSLog::onLogUpdated);

    // connect Parameters
    connect(_vehicle->getParameters(), &Parameters::sendMessageRequest, packetizer, &BasePacketizer::onSendMessageRequest);
    connect(_vehicle->getParameters(), &Parameters::newParameterReceived, &_parameterList, &ParameterList::onParameterUpdate);
    connect(_vehicle->getParameters(), &Parameters::parameterUpdated, &_parameterList, &ParameterList::onParameterUpdate);

    // connect system interactions
    connect(_vehicle->getParameters(), &Parameters::parametersPullingStarted, _vehicle->getTelemetry(), &VehicleTelemetry::onStopTelemetry);
    connect(_vehicle->getParameters(), &Parameters::parametersPullingCompleted, _vehicle->getTelemetry(), &VehicleTelemetry::onStartTelemetry);

    // connect LogsWindow
    connect(&_logsWindow, &LogsWindow::requestLogList, _vehicle->getLogs(), &VehicleLogs::onLogsListRequested);
    connect(&_logsWindow, &LogsWindow::requestLogDownload, _vehicle->getLogs(), &VehicleLogs::onLogDownloadRequested);
    connect(&_logsWindow, &LogsWindow::requestLogDownloadStop, _vehicle->getLogs(), &VehicleLogs::onLogDownloadStopRequested);
    connect(&_logsWindow, &LogsWindow::requestLogErase, _vehicle->getLogs(), &VehicleLogs::onEraseLogsRequested);

    // connect ParameterList
    connect(&_parameterList, &ParameterList::parameterSetRequest, _vehicle->getParameters(), &Parameters::onSendSingleToVehicle);
    connect(&_parameterList, &ParameterList::parametersSetRequest, _vehicle->getParameters(), &Parameters::onSendToVehicle);
    connect(&_parameterList, &ParameterList::parameterReadRequest, _vehicle->getParameters(), &Parameters::onRequestSingleFromVehicle);
    connect(&_parameterList, &ParameterList::parametersReadRequest, _vehicle->getParameters(), &Parameters::onRequestAllFromVehicle);

    // connect serial scanner
    connect(ui->serialScanner, QOverload<QSerialPortInfo>::of(&SerialScanner::connectSerialDevice), this,
        [this](QSerialPortInfo portInfo) {
            qDebug() << "trying to connect serial device";
        }
    );
    connect(ui->serialScanner, QOverload<QSerialPortInfo, int32_t, uint8_t, uint8_t, uint8_t, uint8_t>::of(&SerialScanner::connectSerialDevice), this,
        [this](QSerialPortInfo portInfo) {
            qDebug() << "trying to connect serial device with parameters";
        }
    );

    // connect VehicleManager
    // connect(&_defaultDeviceWidget, &DefaultDeviceWidget::makeDefaultDeviceActive, _mavlinkContext, &MavlinkContext::onMakeDeviceActive);
    // connect(ui->devicesTable, &DevicesTable::makeDeviceActive, _mavlinkContext, &MavlinkContext::onMakeDeviceActive);
    // connect(_mavlinkContext, &MavlinkContext::deviceConnected, ui->devicesTable, &DevicesTable::onDeviceConnected);
    // connect(_mavlinkContext, &MavlinkContext::deviceDisconnected, ui->devicesTable, &DevicesTable::onDeviceDisconnected);
    // connect(_mavlinkContext, &MavlinkContext::deviceStateChanged, ui->devicesTable, &DevicesTable::onDeviceStateChanged);
    // connect(_mavlinkContext, &MavlinkContext::activeDeviceChanged, &_defaultDeviceWidget, &DefaultDeviceWidget::onActiveDeviceChanged);
    // connect(_mavlinkContext, &MavlinkContext::activeDeviceChanged, &_logsWindow, &LogsWindow::onActiveDeviceChanged);
    // connect(_mavlinkContext, &MavlinkContext::activeDeviceChanged, &_parameterList, &ParameterList::onActiveDeviceChanged);

}

MainWindow::~MainWindow() {
    // _mavlinkThread.quit();
    // _mavlinkThread.wait();
    delete ui;
}

void MainWindow::on_actionParameters_set_triggered() {
    if (_parameterList.isHidden())
        _parameterList.show();
}


void MainWindow::on_actionRefresh_configs_triggered() {
    // _mavlinkContext->loadModes();
}

void MainWindow::onParamUpdated(const mavlink_param_value_t& param) {
    // _parameterList.handleMavlink(param);
}

void MainWindow::onParamExtUpdated(const mavlink_param_value_t& param) {
    // _parameterList.handleMavlink(param);
}

void MainWindow::paramsRequested(const mavlink_message_t& msg) {
    emit paramsRequest(msg);
}

void MainWindow::setParamRequested(const mavlink_message_t& msg) {
    emit setParamRequest(msg);
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
