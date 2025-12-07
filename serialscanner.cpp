#include "serialscanner.h"
#include "ui_serialscanner.h"

Q_DECLARE_METATYPE(QSerialPortInfo)

SerialScanner::SerialScanner(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialScanner)
{
    ui->setupUi(this);

    connect(&_portsWatchdog, &QTimer::timeout, this, [this]() {
        QList<QSerialPortInfo> serialInfos = QSerialPortInfo::availablePorts();

        //ui->serial->clear();
        //for(auto& portInfo : _serialInfos) {
        //    ui->serial->addItem(portInfo.portName(), QVariant::fromValue(portInfo));
        //}
    });

    _portsWatchdog.start(500);
}

SerialScanner::~SerialScanner()
{
    delete ui;
}

void SerialScanner::on_serial_currentIndexChanged(int index)
{

}

