#ifndef CONNECTEDDEVICES_H
#define CONNECTEDDEVICES_H

#include <QWidget>

namespace Ui {
class ConnectedDevices;
}

class ConnectedDevices : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectedDevices(QWidget *parent = nullptr);
    ~ConnectedDevices();

private:
    Ui::ConnectedDevices *ui;
};

#endif // CONNECTEDDEVICES_H
