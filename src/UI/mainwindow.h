#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>

#include "mavlink/mavlinkcontext.h"
#include "parameters/parameterlist.h"
#include "logs/logswindow.h"
#include "deviceManagement/defaultdevicewidget.h"
#include "telemetry/telemetrywindow.h"

#include "mavlink/statustext.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class SerialScanner;
class Vehicle;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    Vehicle* _vehicle = nullptr;

    ParameterList _parameterList;
    LogsWindow _logsWindow;
    DefaultDeviceWidget _defaultDeviceWidget;
    TelemetryWindow _telemetry;

    MavlinkContext* _mavlinkContext;
    QThread _mavlinkThread;
    StatusText _statusText;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void paramsRequest(const mavlink_message_t& msg);
    void setParamRequest(const mavlink_message_t& msg);

private slots:
    void on_actionParameters_set_triggered();
    void on_actionRefresh_configs_triggered();
    void onParamUpdated(const mavlink_param_value_t& param);
    void onParamExtUpdated(const mavlink_param_value_t& param);

    void paramsRequested(const mavlink_message_t& msg);
    void setParamRequested(const mavlink_message_t& msg);

    void on_actionOpen_Telemetry_triggered();
    void on_actionDownload_Log_triggered();
    void on_actionReview_Log_triggered();
};
#endif // MAINWINDOW_H
