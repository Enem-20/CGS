#ifndef DEFAULTDEVICEWIDGET_H
#define DEFAULTDEVICEWIDGET_H

#include <QWidget>

namespace Ui {
class DefaultDeviceWidget;
}

class DefaultDeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DefaultDeviceWidget(QWidget *parent = nullptr);
    ~DefaultDeviceWidget();

private slots:
    void on_buttonConnect_clicked();

private:
    Ui::DefaultDeviceWidget *ui;

signals:
    void makeDefaultDeviceActive(QStringView deviceName);

public slots:
    void onActiveDeviceChanged(QStringView deviceName);
};

#endif // DEFAULTDEVICEWIDGET_H
