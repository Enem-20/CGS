#ifndef DEVICES_H
#define DEVICES_H

#include <QWidget>

namespace Ui {
class Devices;
}

class Devices : public QWidget
{
    Q_OBJECT

public:
    explicit Devices(QWidget *parent = nullptr);
    ~Devices();

private:
    void transitToWidget(QWidget* widget);

    Ui::Devices *ui;
    QWidget* _devicesTable;
    QWidget* _addDevice;
public slots:
    void onTransitToAddDevice();
    void onTransitToDevicesTable();
};

#endif // DEVICES_H
