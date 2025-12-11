#ifndef SERIALSCANNER_HFORM
#define SERIALSCANNER_HFORM

#include <QWidget>
#include <QTimer>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class SerialScanner;
}

class QListWidgetItem;

class SerialScanner : public QWidget {
    Q_OBJECT

private:
    QTimer _portsWatchdog;
    static QString _defaultPortMatchPattern;
    QList<QSerialPortInfo> _serialInfos;

private:
    size_t fitToUpdated(const QList<QSerialPortInfo>& compSet);
    void filterNewInfos(QList<QSerialPortInfo>& serialInfos);

public:
    explicit SerialScanner(QWidget *parent = nullptr);
    ~SerialScanner();

private slots:
    void on_serial_currentIndexChanged(int index);
    void on_devices_itemDoubleClicked(QListWidgetItem *item);

signals:
    void connectSerialDevice(QSerialPortInfo portInfo);
    void connectSerialDevice(QSerialPortInfo portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl);

private:
    Ui::SerialScanner *ui;
};

#endif // SERIALSCANNER_HFORM
