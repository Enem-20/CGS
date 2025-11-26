#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QJsonObject>

#include <common/mavlink.h>

#include "udpmavlinkdevice.h"
#include "attitude.h"
#include "localpositionned.h"
#include "statustext.h"

#include "parameterlist.h"
#include "logswindow.h"

class MavlinkContext : public QObject {
    Q_OBJECT
private:

    UDPMavlinkDevice _localDefaultDevice;
    Attitude _attitude;
    LocalPositionNED _lPositionNED;
    StatusText _statusText;
    QTimer _heartBeatTimer;
    mavlink_message_t _heartBeatMsg;
    QJsonObject _existingModes;

public:
    MavlinkContext();
    void sendHeartbeat();
private:
    void updateMode(uint8_t autopilot, uint8_t type, uint32_t customMode);
signals:
    void heartbeatMessageReceived(const mavlink_message_t& heartbeat);
    void heartbeatUpdated(const mavlink_heartbeat_t& heartbeat);
    void modeUpdated(const QString& mode);
    void armedUpdated(const QString& armed);
    void attitudeUpdated(const QString& rollPitchYaw);
    void speedsUpdated(const QString& speeds);
    void logUpdated(const QString& msg, const QString& severity, QColor color);

    void paramUpdated(const mavlink_param_value_t& param);
    void paramExtUpdated(const mavlink_param_ext_value_t& param);
    void logEntryRecieved(const mavlink_log_entry_t& logEntry);
    void logDataRecieved(const mavlink_log_data_t& logData, const mavlink_message_t& msg);
private slots:
    void handleMavlinkMessage(mavlink_message_t msg);
public slots:
    void loadModes();
    void sendCommand(const mavlink_message_t& msg);
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    MavlinkContext _mavlinkContext;
    ParameterList _parameterList;
    LogsWindow _logsWindow;
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
    void on_actionLogs_triggered();
};
#endif // MAINWINDOW_H
