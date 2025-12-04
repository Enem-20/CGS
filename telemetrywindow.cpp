#include "telemetrywindow.h"
#include "ui_telemetrywindow.h"

#include <QRegularExpression>

#include <common/mavlink.h>

#include "plotter.h"

TelemetryWindow::TelemetryWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TelemetryWindow)
{
    ui->setupUi(this);
    _plotter = new Plotter;
}

TelemetryWindow::~TelemetryWindow()
{
    delete ui;
}

void TelemetryWindow::onGlobalPositionIntUpdated(const mavlink_global_position_int_t& msg) {
    if(!_telemetryMap.contains("GLOBAL_POSITION_INT")) [[unlikely]] {
        QHash<QString, TelemetryParam> params;
        params.emplace("alt",   TelemetryParam{"alt",   QVector<float>{static_cast<float>(msg.alt)}});
        params.emplace("hdg",   TelemetryParam{"hdg",   QVector<float>{static_cast<float>(msg.hdg)}});
        params.emplace("lat",   TelemetryParam{"lat",   QVector<float>{static_cast<float>(msg.lat)}});
        params.emplace("lon",   TelemetryParam{"lon",   QVector<float>{static_cast<float>(msg.lon)}});
        params.emplace("ralt",  TelemetryParam{"ralt",  QVector<float>{static_cast<float>(msg.relative_alt)}});
        params.emplace("vx",    TelemetryParam{"vx",    QVector<float>{static_cast<float>(msg.vx)}});
        params.emplace("vy",    TelemetryParam{"vx",    QVector<float>{static_cast<float>(msg.vy)}});
        params.emplace("vz",    TelemetryParam{"vx",    QVector<float>{static_cast<float>(msg.vz)}});

        TelemetryGroup group {
            msg.time_boot_ms,
            "GLOBAL_POSITION_INT",
            params
        };
    } else {
        TelemetryGroup& _telemetryGroup = _telemetryMap.find("GLOBAL_POSITION_INT").value();
        _telemetryGroup._params["alt"].values += static_cast<float>(msg.alt);
        _telemetryGroup._params["hdg"].values += static_cast<float>(msg.hdg);
        _telemetryGroup._params["lat"].values += static_cast<float>(msg.lat);
        _telemetryGroup._params["lon"].values += static_cast<float>(msg.lon);
        _telemetryGroup._params["ralt"].values += static_cast<float>(msg.relative_alt);
        _telemetryGroup._params["vx"].values += static_cast<float>(msg.vx);
        _telemetryGroup._params["vy"].values += static_cast<float>(msg.vy);
        _telemetryGroup._params["vz"].values += static_cast<float>(msg.vz);
    }
}

void TelemetryWindow::onLocalPositionNEDUpdated(const mavlink_local_position_ned_t& msg) {

}

void TelemetryWindow::onAttitudeUpdated(const mavlink_attitude_t& msg) {

}

void TelemetryWindow::onStatusTextUpdated(const mavlink_statustext_t& msg) {
    QString text = msg.text;
    QRegularExpression regex(_customTelemetry);

    QRegularExpressionMatch match = regex.match(text);
    if (match.hasMatch()) {
        quint64 timestamp = match.captured(1).toLongLong();
        QString group = match.captured(2).trimmed();
        QString name = match.captured(3).trimmed();
        QString value = match.captured(4);

        if(!_telemetryMap.contains(group)) [[unlikely]]{
            QHash<QString, TelemetryParam> params;
            QVector<float> values;
            values.append(value.toFloat());
            params.insert(name, TelemetryParam{name, values});

            TelemetryGroup group {
                timestamp,
                "GLOBAL_POSITION_INT",
                params
            };
        }
         else {
            TelemetryGroup& _telemetryGroup = _telemetryMap.find(group).value();
            if(_telemetryGroup._params.contains(name))
                _telemetryGroup._params[name].values.append(value.toFloat());
            else
                _telemetryGroup._params.emplace(name, TelemetryParam{ name, QVector<float>{value.toFloat()}});
        }
    }

}

void TelemetryWindow::onHeartbeatUpdated(const mavlink_heartbeat_t& msg) {

}

void TelemetryWindow::onParamValueUpdated(const mavlink_param_value_t& msg) {

}

void TelemetryWindow::onParamExtValueUpdated(const mavlink_param_ext_value_t& msg) {

}

void TelemetryWindow::onParamExtAckUpdated(const mavlink_param_ext_ack_t& msg) {

}

void TelemetryWindow::onLogEntryUpdated(const mavlink_log_entry_t& msg) {

}

void TelemetryWindow::onLogDataUpdated(const mavlink_log_data_t& msg) {

}
