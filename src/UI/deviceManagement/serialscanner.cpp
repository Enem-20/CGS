#include "serialscanner.h"
#include "ui_serialscanner.h"

#include <QRegularExpression>

#include "src/deviceManagement/serial/serialdevice.h"
#include "src/deviceManagement/DeviceManager.h"

Q_DECLARE_METATYPE(QSerialPortInfo)

QString SerialScanner::_defaultPortMatchPattern = "^tty(USB|AMA|ACM)\\d+$";

size_t SerialScanner::fitToUpdated(const QList<QSerialPortInfo>& compSet) {
    size_t updated = 0;
    for (const auto& portInfo : compSet) {
        if (!std::any_of(_serialInfos.constBegin(), _serialInfos.constEnd(), [&portInfo](const QSerialPortInfo& _portInfo){
            return portInfo.portName() == _portInfo.portName();
        })) {
            _serialInfos += portInfo;
            ++updated;
        }
    }
    _serialInfos.removeIf([&compSet, &updated](const QSerialPortInfo& _portInfo){
        bool result = !std::any_of(compSet.constBegin(), compSet.constEnd(), [&_portInfo](const QSerialPortInfo& portInfo){
            return portInfo.portName() == _portInfo.portName();
        });
        updated += (result) ? 1 : 0;
        return result;
    });

    return updated;
}

void SerialScanner::filterNewInfos(QList<QSerialPortInfo>& serialInfos) {
    serialInfos.removeIf([this](const QSerialPortInfo& serialInfo){
        QRegularExpression re(ui->filter->text());
        return !re.match(serialInfo.portName()).hasMatch();
    });
}

SerialScanner::SerialScanner(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialScanner)
{
    ui->setupUi(this);
    ui->filter->setText(_defaultPortMatchPattern);
    connect(&_portsWatchdog, &QTimer::timeout, this, [this]() {
        QList<QSerialPortInfo> portInfos = QSerialPortInfo::availablePorts();
        filterNewInfos(portInfos);
        size_t fitCount = fitToUpdated(portInfos);
        if (fitCount) {
            ui->devices->clear();
            for (auto& portInfo : _serialInfos) {
                QListWidgetItem* item = new QListWidgetItem(
                    (!portInfo.description().isEmpty()) ? portInfo.portName() + " " + portInfo.description() : portInfo.portName());

                ui->devices->addItem(item);
            }
        }
    });

    _portsWatchdog.start(500);

    connect(this, QOverload<QSerialPortInfo>::of(&SerialScanner::connectSerialDevice), this,
            [this](QSerialPortInfo portInfo) {
                qDebug() << "trying to connect serial device";

                SerialDevice* device = DeviceManager::getInstance()->createSerialDevice(portInfo.portName(), portInfo);
                QMetaObject::invokeMethod(device, "onSetupPort", Qt::QueuedConnection,
                                          Q_ARG(const QSerialPortInfo&, portInfo)
                                          );
            }
            );
    connect(this, QOverload<QSerialPortInfo, int32_t, uint8_t, uint8_t, uint8_t, uint8_t>::of(&SerialScanner::connectSerialDevice), this,
            [this](QSerialPortInfo portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl) {
                qDebug() << "trying to connect serial device with parameters";

                SerialDevice* device = DeviceManager::getInstance()->createSerialDevice(portInfo.portName(), portInfo);
                QMetaObject::invokeMethod(device, "onSetupPort", Qt::QueuedConnection,
                                          Q_ARG(const QSerialPortInfo&, portInfo), Q_ARG(int32_t, baudRate),
                                          Q_ARG(uint8_t, dataBits), Q_ARG(uint8_t, stopBits),
                                          Q_ARG(uint8_t, parity), Q_ARG(uint8_t, flowControl)
                                          );
            }
            );
}

SerialScanner::~SerialScanner() {
    delete ui;
}

void SerialScanner::on_serial_currentIndexChanged(int index) {

}

void SerialScanner::on_devices_itemDoubleClicked(QListWidgetItem *item) {
    item->setBackground(QColor::fromRgb(0,255,0));
    int32_t baudRate = ui->baudrate->text().toInt();
    uint8_t dataBits = ui->dataBits->currentText().toUInt();
    uint8_t stopBits = ui->stopBits->currentText().toUInt();
    uint8_t parity = ui->parity->currentText().toUInt();
    uint8_t flowControl = ui->flowControl->currentText().toUInt();
    emit connectSerialDevice(_serialInfos[ui->devices->row(item)], baudRate, dataBits, stopBits, parity, flowControl);
}
