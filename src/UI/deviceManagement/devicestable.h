#ifndef DEVICESTABLE_H
#define DEVICESTABLE_H

#include <QWidget>
#include <QTableWidget>

#include "mavlink/mavlinkdevice.h"

namespace Ui {
class DevicesTable;
}

class DevicesTable : public QWidget {
    Q_OBJECT

public:
    explicit DevicesTable(QWidget *parent = nullptr);
    ~DevicesTable();

private:
    Ui::DevicesTable *ui;

signals:
    void makeDeviceActive(QStringView deviceName);

public slots:
    void onDeviceConnected(QStringView name, QStringView type);
    void onDeviceDisconnected(QStringView name);
    void onDeviceStateChanged(QStringView name, PortState state);

private slots:
    void on_table_itemDoubleClicked(QTableWidgetItem *item);
};

#endif // DEVICESTABLE_H
