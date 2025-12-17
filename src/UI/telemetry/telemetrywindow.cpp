#include "telemetrywindow.h"
#include "ui_telemetrywindow.h"

#include <QRegularExpression>
#include <QPushButton>
#include "UI/plotter/plotgroup.h"
#include "UI/plotter/plot.h"

#include "UI/plotter/plotter.h"

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

void TelemetryWindow::createParam(TelemetryGroup& group, const QString& name, bool active) {
    QTreeWidgetItem* treeItem = new QTreeWidgetItem();
    treeItem->setText(0, name);
    treeItem->setFlags(treeItem->flags() | Qt::ItemIsUserCheckable);
    treeItem->setCheckState(0, active ? Qt::Checked : Qt::Unchecked);

    Plot* plot = nullptr;
    if (active) {
        plot = group.plotGroup->createPlot(name, "time(ms)", name, {group.timestamp, group.timestamp + 10});
        plot->createGraph(name);
    }

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
    TelemetryGroup& group = _telemetryMap[groupName];
    TelemetryParam& param = group._params[paramName];

    if (group.timeValues.size() == 0 || param.plot == nullptr) {
        return;
    }

    qsizetype startIndex = group.timeValues.size() - 1;
    while (true) {
        if (startIndex == 0) {
            break;
        }
        if (static_cast<double>(group.timeValues[startIndex]) < (t - _timeRangeMillis)) {
            break;
        }
        startIndex--;
    }

    float min = std::numeric_limits<float>::infinity();
    float max = -std::numeric_limits<float>::infinity();

    for (qsizetype i = startIndex; i < param.values.size(); i++) {
        min = std::min(min, param.values[i]);
        max = std::max(max, param.values[i]);
    }

    if (min > max) {
        min = -1.0f;
        max = 1.0f;
    }

    const double minRange = 0.001;
    double range = static_cast<double>(max) - static_cast<double>(min);
    if (range < minRange) {
        double change = minRange - range;
        range = minRange;
        min -= change / 2.0;
        max += change / 2.0;
    }
    double padding = static_cast<double>(range) * _rangeYPadding;

    Plot* plot = param.plot;
    plot->addPoint(paramName, QPair<double, double>(t, value));
    plot->setXRange(QPair<double, double>{t - _timeRangeMillis, t});
    plot->setYRange(QPair<double, double>{static_cast<double>(min) - padding, static_cast<double>(max) + padding});
    plot->replot();
}

void TelemetryWindow::handleTelemetrySelectionChanged(QTreeWidgetItem *item, int column) {
    QString paramName = item->text(column);
    QString groupName = item->parent()->text(column);

    if (item->checkState(column)) {
        _telemetryMap[groupName]._params[paramName].plot = _plotter->createPlot(groupName, paramName, "time(ms)", paramName);
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

void TelemetryWindow::onVehicleOrientationUpdated(const QVector3D& orientation, uint64_t timestamp) {
    static const QString groupName("GLOBAL_POSITION_INT");

    if (!_telemetryMap.contains(groupName)) [[unlikely]] {
        TelemetryGroup& group = createGroup(groupName, timestamp);
    }

    TelemetryGroup& group = _telemetryMap.find(groupName).value();

    if (!parameterExists(group, "roll")) [[unlikely]] {
        createParam(group, "roll");
        createParam(group, "pitch");
        createParam(group, "yaw");
    }

    group._params["roll"].values += orientation.x();
    group._params["pitch"].values += orientation.y();
    group._params["yaw"].values += orientation.z();

    const double t = static_cast<double>(timestamp - group.timestamp);
    plotValue(groupName, "roll", t, static_cast<double>(orientation.x()));
    plotValue(groupName, "pitch", t, static_cast<double>(orientation.y()));
    plotValue(groupName, "yaw", t, static_cast<double>(orientation.z()));
}

void TelemetryWindow::onVehicleVelocityUpdated(const QVector3D& velocity, uint64_t timestamp) {
    static const QString groupName("GLOBAL_POSITION_INT");

    if (!_telemetryMap.contains(groupName)) [[unlikely]] {
        TelemetryGroup& group = createGroup(groupName, timestamp);
    }

    TelemetryGroup& group = _telemetryMap.find(groupName).value();

    if (!parameterExists(group, "vx")) [[unlikely]] {
        createParam(group, "vx");
        createParam(group, "vy");
        createParam(group, "vz");
    }

    group._params["vx"].values += velocity.x();
    group._params["vy"].values += velocity.y();
    group._params["vz"].values += velocity.z();

    const double t = static_cast<double>(timestamp - group.timestamp);
    plotValue(groupName, "vx", t, static_cast<double>(velocity.x()));
    plotValue(groupName, "vy", t, static_cast<double>(velocity.y()));
    plotValue(groupName, "vz", t, static_cast<double>(velocity.z()));
}

void TelemetryWindow::onVehicleAltitudeUpdated(float altitude, uint64_t timestamp) {
    static const QString groupName("GLOBAL_POSITION_INT");

    if (!_telemetryMap.contains(groupName)) [[unlikely]] {
        TelemetryGroup& group = createGroup(groupName, timestamp);
    }

    TelemetryGroup& group = _telemetryMap.find(groupName).value();

    if (!parameterExists(group, "alt")) [[unlikely]] {
        createParam(group, "alt");
    }

    group._params["alt"].values += altitude;

    const double t = static_cast<double>(timestamp - group.timestamp);
    plotValue(groupName, "vx", t, static_cast<double>(altitude));
}

void TelemetryWindow::onStatusMessageReceived(const QString& text, const QString& severity) {
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
