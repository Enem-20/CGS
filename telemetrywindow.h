#ifndef TELEMETRYWINDOW_H
#define TELEMETRYWINDOW_H

#include <QWidget>
#include <QTreeWidget>
#include <QVector>

#include "plotgroup.h"
#include "plot.h"

struct __mavlink_global_position_int_t;
typedef __mavlink_global_position_int_t mavlink_global_position_int_t;

struct __mavlink_local_position_ned_t;
typedef __mavlink_local_position_ned_t mavlink_local_position_ned_t;

struct __mavlink_attitude_t;
typedef __mavlink_attitude_t mavlink_attitude_t;

struct __mavlink_statustext_t;
typedef __mavlink_statustext_t mavlink_statustext_t;

struct __mavlink_message;
typedef __mavlink_message mavlink_message_t;

struct __mavlink_heartbeat_t;
typedef __mavlink_heartbeat_t mavlink_heartbeat_t;

struct __mavlink_param_value_t;
typedef __mavlink_param_value_t mavlink_param_value_t;

struct __mavlink_param_ext_value_t;
typedef __mavlink_param_ext_value_t mavlink_param_ext_value_t;

struct __mavlink_param_ext_ack_t;
typedef __mavlink_param_ext_ack_t mavlink_param_ext_ack_t;

struct __mavlink_log_entry_t;
typedef __mavlink_log_entry_t mavlink_log_entry_t;

struct __mavlink_log_data_t;
typedef __mavlink_log_data_t mavlink_log_data_t;

namespace Ui {
class TelemetryWindow;
}

class Plotter;

struct TelemetryParam {
    QString name;
    QVector<float> values;
    QTreeWidgetItem* treeItem;
    Plot* plot;
};

struct TelemetryGroup {
    quint64 timestamp;
    QString groupName;
    QHash<QString, TelemetryParam> _params;
    QTreeWidgetItem* treeItem;
    PlotGroup* plotGroup;
    QVector<float> timeValues;
};

class TelemetryWindow : public QWidget
{
    Q_OBJECT
private:
    Plotter* _plotter;
    QHash<QString, TelemetryGroup> _telemetryMap;
    QString _customTelemetry = "^(\\d+):\\s*([^:]+?)\\s*:\\s*([^:]+?)\\s*:\\s*([-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)";
    Ui::TelemetryWindow *ui;
    double _timeRangeMillis = 30000.0;
    double _rangeYPadding = 0.05;

private:
    bool groupExists(const QString& groupName);
    bool parameterExists(const QString& groupName, const QString& paramName);
    bool parameterExists(const TelemetryGroup& group, const QString& paramName);
    TelemetryGroup& createGroup(const QString& name, uint64_t timestamp);
    void createParam(TelemetryGroup& group, const QString& name, bool active = false);
    void plotValue(const QString& groupName, const QString& paramName, double t, double value);
    void handleTelemetrySelectionChanged(QTreeWidgetItem *item, int column);

public:
    explicit TelemetryWindow(QWidget *parent = nullptr);
    ~TelemetryWindow();

signals:
    void updated(const QStringList& telemetryQueue);

public slots:
    void onGlobalPositionIntUpdated(const mavlink_global_position_int_t& msg);
    void onLocalPositionNEDUpdated(const mavlink_local_position_ned_t& msg);
    void onAttitudeUpdated(const mavlink_attitude_t& msg);
    void onStatusTextUpdated(const mavlink_statustext_t& msg);
    void onHeartbeatUpdated(const mavlink_heartbeat_t& msg);
    void onParamValueUpdated(const mavlink_param_value_t& msg);
    void onParamExtValueUpdated(const mavlink_param_ext_value_t& msg);
    void onParamExtAckUpdated(const mavlink_param_ext_ack_t& msg);
    void onLogEntryUpdated(const mavlink_log_entry_t& msg);
    void onLogDataUpdated(const mavlink_log_data_t& msg);
};

#endif // TELEMETRYWINDOW_H
