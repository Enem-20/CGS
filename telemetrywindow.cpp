#include "telemetrywindow.h"
#include "ui_telemetrywindow.h"

#include <QRegularExpression>
#include <QPushButton>
#include "plotgroup.h"
#include "plot.h"

#include <common/mavlink.h>

#include "plotter.h"

bool TelemetryWindow::groupExists(const QString& groupName) {
    return _telemetryMap.contains(groupName);
}

bool TelemetryWindow::parameterExists(const QString& groupName, const QString& paramName) {
    return groupExists(groupName) && _telemetryMap[groupName]._params.contains(paramName);
}

bool TelemetryWindow::parameterExists(const TelemetryGroup& group, const QString& paramName) {
    return group._params.contains(paramName);
}

TelemetryGroup& TelemetryWindow::createGroup(const QString& groupName, uint64_t timestamp) {
    if (!_telemetryMap.contains(groupName)) {
        PlotGroup* plotGroup = _plotter->createPlotGroup(groupName);
        QTreeWidgetItem* treeItem = new QTreeWidgetItem();
        treeItem->setText(0, groupName);

        ui->telemetryTree->addTopLevelItem(treeItem);

        _telemetryMap.emplace(groupName,
            TelemetryGroup{
                timestamp,
                groupName,
                {},
                treeItem,
                plotGroup
            }
        );
    }
    return _telemetryMap[groupName];
}

void TelemetryWindow::createParam(TelemetryGroup& group, const QString& name) {
    QTreeWidgetItem* treeItem = new QTreeWidgetItem();
    treeItem->setText(0, name);
    treeItem->setFlags(treeItem->flags() | Qt::ItemIsUserCheckable);
    treeItem->setCheckState(0, Qt::Checked);

    Plot* plot = group.plotGroup->createPlot(name, "t", name, {group.timestamp, group.timestamp + 10});
    plot->createGraph(name);

    group.treeItem->addChild(treeItem);

    group._params.emplace(name,
        TelemetryParam {
            name,
            {},
            treeItem,
            plot
        }
    );
}

void TelemetryWindow::plotValue(const QString& groupName, const QString& paramName, double t, double value) {
    if (!parameterExists(groupName, paramName)) {
        return;
    }
    if (_telemetryMap[groupName]._params[paramName].plot == nullptr) {
        return;
    }
    _plotter->addPoint(groupName, paramName, paramName, QPair<double, double>(t, value));
}

void TelemetryWindow::handleTelemetrySelectionChanged(QTreeWidgetItem *item, int column) {
    QString paramName = item->text(column);
    QString groupName = item->parent()->text(column);

    if (item->checkState(column)) {
        _telemetryMap[groupName]._params[paramName].plot = _plotter->createPlot(groupName, paramName, "t", paramName);
        _telemetryMap[groupName]._params[paramName].plot->createGraph(paramName);
    }
    else {
        _telemetryMap[groupName]._params[paramName].plot = nullptr;
        _telemetryMap[groupName]._params[paramName].values.clear();
        _plotter->removePlot(groupName, paramName);
    }

    _plotter->replot();
}

TelemetryWindow::TelemetryWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TelemetryWindow)
{
    ui->setupUi(this);
    _plotter = new Plotter(ui->plotterFrame);
    ui->plotterFrame->layout()->addWidget(_plotter);

    connect(ui->telemetryTree, &QTreeWidget::itemChanged, this, &TelemetryWindow::handleTelemetrySelectionChanged);
}

TelemetryWindow::~TelemetryWindow() {
    delete ui;
}

void TelemetryWindow::onGlobalPositionIntUpdated(const mavlink_global_position_int_t& msg) {
    static const QString groupName("GLOBAL_POSITION_INT");

    if (!_telemetryMap.contains(groupName)) [[unlikely]] {
        TelemetryGroup& group = createGroup(groupName, static_cast<uint64_t>(msg.time_boot_ms));
        createParam(group, "alt");
        createParam(group, "hdg");
        createParam(group, "lat");
        createParam(group, "lon");
        createParam(group, "ralt");
        createParam(group, "vx");
        createParam(group, "vy");
        createParam(group, "vz");
    }

    TelemetryGroup& _telemetryGroup = _telemetryMap.find(groupName).value();
    _telemetryGroup._params["alt"].values += static_cast<float>(msg.alt);
    _telemetryGroup._params["hdg"].values += static_cast<float>(msg.hdg);
    _telemetryGroup._params["lat"].values += static_cast<float>(msg.lat);
    _telemetryGroup._params["lon"].values += static_cast<float>(msg.lon);
    _telemetryGroup._params["ralt"].values += static_cast<float>(msg.relative_alt);
    _telemetryGroup._params["vx"].values += static_cast<float>(msg.vx);
    _telemetryGroup._params["vy"].values += static_cast<float>(msg.vy);
    _telemetryGroup._params["vz"].values += static_cast<float>(msg.vz);

    const double t = static_cast<double>(msg.time_boot_ms);
    plotValue(groupName, "alt", t, static_cast<double>(msg.alt));
    plotValue(groupName, "hdg", t, static_cast<double>(msg.hdg));
    plotValue(groupName, "lat", t, static_cast<double>(msg.lat));
    plotValue(groupName, "lon", t, static_cast<double>(msg.lon));
    plotValue(groupName, "ralt", t, static_cast<double>(msg.relative_alt));
    plotValue(groupName, "vx", t, static_cast<double>(msg.vx));
    plotValue(groupName, "vy", t, static_cast<double>(msg.vy));
    plotValue(groupName, "vz", t, static_cast<double>(msg.vz));

    _plotter->replot();
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
        QString groupName = match.captured(2).trimmed();
        QString paramName = match.captured(3).trimmed();
        QString value = match.captured(4);

        if (!groupExists(groupName)) [[unlikely]] {
            createGroup(groupName, timestamp);
        }

        TelemetryGroup& group = _telemetryMap[groupName];
        if (!parameterExists(group, paramName)) [[unlikely]] {
            createParam(group, paramName);
        }

        group._params[paramName].values += value.toFloat();
        plotValue(groupName, paramName, static_cast<double>(timestamp), value.toDouble());
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
