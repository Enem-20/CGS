#ifndef ADDDEVICE_H
#define ADDDEVICE_H

#include <QWidget>

namespace Ui {
class AddDevice;
}

class AddDevice : public QWidget
{
    Q_OBJECT

public:
    explicit AddDevice(QWidget *parent = nullptr);
    ~AddDevice();

private:
    Ui::AddDevice *ui;
signals:
    void transitToDevicesTable();
private slots:
    void on_cancel_clicked();
    void on_ok_clicked();
};

#endif // ADDDEVICE_H
