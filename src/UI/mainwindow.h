#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>

#include "channelManagement/RemoteChannelID.h"
#include "parameters/parameterlist.h"
#include "logs/logswindow.h"
#include "telemetry/telemetrywindow.h"

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

    Vehicle* _activeVehicle = nullptr;
    LogsWindow _logsWindow;
    TelemetryWindow _telemetryWindow;
    ParameterList _parameterList;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionParameters_set_triggered();
    void on_actionRefresh_configs_triggered();
    void on_actionOpen_Telemetry_triggered();
    void on_actionDownload_Log_triggered();
    void on_actionReview_Log_triggered();

    void onDeviceCreated(uint64_t id);
    void onActiveChannelChanged(RemoteChannelID id);
};

#endif // MAINWINDOW_H
