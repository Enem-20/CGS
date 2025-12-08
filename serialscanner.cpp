#include "serialscanner.h"
#include "ui_serialscanner.h"

#include <QRegularExpression>

Q_DECLARE_METATYPE(QSerialPortInfo)

QString SerialScanner::_defaultPortMatchPattern = "^tty(USB|AMA|ACM)\\d+$";

size_t SerialScanner::fitToUpdated(const QList<QSerialPortInfo>& compSet) {
    size_t updated = 0;
    for(const auto& portInfo : compSet) {
        if(!std::any_of(_serialInfos.constBegin(), _serialInfos.constEnd(), [&portInfo](const QSerialPortInfo& _portInfo){
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
        if(fitCount) {
            ui->serial->clear();
            for(auto& portInfo : _serialInfos) {
                ui->serial->addItem(portInfo.portName() + " " + portInfo.description(), QVariant::fromValue(portInfo));
            }
        }
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

