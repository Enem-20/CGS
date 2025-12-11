#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>

#include "mavlink/mavlinkpacketizer.h"
#include "mavlink/mavlinkcontext.h"
#include "parameters/parameterlist.h"
#include "logs/logswindow.h"
#include "deviceManagement/defaultdevicewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class TelemetryWindow;
class SerialScanner;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    MavlinkContext* _mavlinkContext;
    ParameterList _parameterList;
    LogsWindow _logsWindow;
    DefaultDeviceWidget _defaultDeviceWidget;
    TelemetryWindow* _telemetry;
    QThread _mavlinkThread;
    Ui::MainWindow *ui;

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
