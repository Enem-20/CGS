#ifndef SERIALSCANNER_HFORM
#define SERIALSCANNER_HFORM

#include <QWidget>
#include <QTimer>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class SerialScanner;
}

class SerialScanner : public QWidget
{
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
private:
    Ui::SerialScanner *ui;
};

#endif // SERIALSCANNER_HFORM
