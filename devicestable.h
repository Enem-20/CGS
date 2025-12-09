#ifndef DEVICESTABLE_H
#define DEVICESTABLE_H

#include <QWidget>

#include "mavlinkdevice.h"

namespace Ui {
class DevicesTable;
}

class DevicesTable : public QWidget
{
    Q_OBJECT

public:
    explicit DevicesTable(QWidget *parent = nullptr);
    ~DevicesTable();

private:
    Ui::DevicesTable *ui;

public slots:
    void onDeviceConnected(MavlinkDevice* device);
    void onDeviceDisconnected(MavlinkDevice* device);
    void onDeviceStateChanged(MavlinkDevice* device, PortState state);
};

#endif // DEVICESTABLE_H
