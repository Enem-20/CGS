#ifndef DEVICESTABLE_H
#define DEVICESTABLE_H

#include <QWidget>
#include <QTableWidget>

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
private:
    void showAll();
    void hideAll();
signals:
    void makeDeviceActive(QStringView deviceName);
    void transitToAddDevice();
public slots:
    void onDeviceConnected(QStringView name, QStringView type);
    void onDeviceDisconnected(QStringView name);
    void onDeviceStateChanged(QStringView name);

private slots:
    void on_table_itemDoubleClicked(QTableWidgetItem *item);
    void on_add_clicked();
};

#endif // DEVICESTABLE_H
