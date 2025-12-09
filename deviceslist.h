#ifndef DEVICESLIST_H
#define DEVICESLIST_H

#include <QWidget>

namespace Ui {
class DevicesList;
}

class DevicesList : public QWidget
{
    Q_OBJECT

public:
    explicit DevicesList(QWidget *parent = nullptr);
    ~DevicesList();

private:
    Ui::DevicesList *ui;
};

#endif // DEVICESLIST_H
