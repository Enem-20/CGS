#include "plotter.h"
#include "ui_plotter.h"

#include <QResizeEvent>

#include "qcustomplot.h"

#include "plotgroup.h"

Plotter::Plotter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Plotter)
{
    ui->setupUi(this);
    ui->plots->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAttribute(Qt::WA_PendingResizeEvent);
}

Plotter::~Plotter()
{
    delete ui;
}

void Plotter::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    for(auto group : _plotGroups.values()) {
        group->resize(size().width(), size().height() / _plotGroups.size());
    }
}

QCPGraph* Plotter::getGraph(const QString& groupName, const QString& plotName, const QString& name) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        return groupIt.value()->getGraph(plotName, name);
    }

    qWarning() << "Graph " << name << " missing due to missing group " << groupName;
    return nullptr;
}

Plot* Plotter::getPlot(const QString& groupName, const QString& name) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        return groupIt.value()->getPlot(name);
    }

    qWarning() << "Plot " << name << " missing due to missing group " << groupName;
    return nullptr;
}

PlotGroup* Plotter::getGroup(const QString& name) {
    auto groupIt = _plotGroups.find(name);
    if(groupIt != _plotGroups.end()) {
        return groupIt.value();
    }

    qWarning() << "Group " << name << " missing";
    return nullptr;
}

void Plotter::removeGraph(const QString& groupName, const QString& plotName, const QString& name) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->removeGraph(plotName, name);
        return;
    }
    qWarning() << "Can't remove graph " << name << " due to missing group " << groupName;
}

void Plotter::removePlot(const QString& groupName, const QString& name) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->removePlot(name);
        return;
    }
    qWarning() << "Can't remove plot " << name << " due to missing group " << groupName;
}

void Plotter::removeGroup(const QString& name) {
    auto groupIt = _plotGroups.find(name);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->deleteLater();
        return;
    }
}

void Plotter::clear() {
    for(PlotGroup* group : _plotGroups) {
        group->deleteLater();
    }
    _plotGroups.clear();
}

void Plotter::replotExactGroup(const QString& name) {
    auto groupIt = _plotGroups.find(name);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->replot();
        return;
    }
    qWarning() << "Can't replot group " << name << ": it's missing";
}

void Plotter::replotExactPlot(const QString& groupName, const QString& name) {
    auto groupIt = _plotGroups.find(name);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->replotExact(name);
        return;
    }
    qWarning() << "Can't replot plot " << name << ": group " << groupName << " is missing";
}

void Plotter::replot() {
    for(PlotGroup* group : _plotGroups) {
        group->replot();
    }
}

PlotGroup* Plotter::createPlotGroup(const QString& name) {
    auto groupIt = _plotGroups.find(name);
    if(groupIt == _plotGroups.end()) {
        PlotGroup* group = new PlotGroup(name, ui->plots);
        _plotGroups.emplace(name, group);
        return group;
    }
    qWarning() << "Plot group " << name << " already exists";
    return groupIt.value();
}

Plot* Plotter::createPlot(const QString& groupName, const QString& name,
                         const QString& horzAxisName, const QString& vertAxisName,
                         std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        return groupIt.value()->createPlot(name, horzAxisName, vertAxisName, xRange, yRange);
    } else {
        createPlotGroup(groupName);
        groupIt = _plotGroups.find(groupName);
        return groupIt.value()->createPlot(name, horzAxisName, vertAxisName, xRange, yRange);
    }
}

QCPGraph* Plotter::createGraph(const QString& groupName, const QString& plotName, const QString& name) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        return groupIt.value()->createGraph(plotName, name);
    } else {
        qWarning() << "Missing group " << groupName << " and plot " << plotName << ". Create them first, then try again";
    }
}

void Plotter::setData(const QString& groupName, const QString& plotName, const QString& graphName,
                      const QVector<double>& keys, const QVector<double>& values) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setData(plotName, graphName, keys, values);
    } else {
        qWarning() << "Failed to set data for " << groupName << " and plot " << plotName << ". Create them first, then try again";
    }
}

void Plotter::addData(const QString& groupName, const QString& plotName, const QString& graphName,
                      const QVector<double>& keys, const QVector<double>& values) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->addData(plotName, graphName, keys, values);
    } else {
        qWarning() << "Failed to add data for " << groupName << " and plot " << plotName << ". Create them first, then try again";
    }
}

void Plotter::addPoint(const QString& groupName, const QString& plotName, const QString& graphName,
                       const QPair<double, double>& point) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->addPoint(plotName, graphName, point);
    } else {
        qWarning() << "Failed to add point for " << groupName << " and plot " << plotName << ". Create them first, then try again";
    }
}

void Plotter::setXRangePlot(const QString& groupName, const QString& name, std::pair<int64_t, int64_t> range) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setXRangePlot(name, range);
        return;
    }

    qWarning() << "Failed to change x range for plot " << name << ": missing parent group " << groupName;
}

void Plotter::setYRangePlot(const QString& groupName, const QString& name, std::pair<int64_t, int64_t> range) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setYRangePlot(name, range);
        return;
    }

    qWarning() << "Failed to change y range for plot " << name << ": missing parent group " << groupName;
}

void Plotter::setRangesPlot(const QString& groupName, const QString& name, std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setXRangePlot(name, xRange);
        groupIt.value()->setXRangePlot(name, yRange);
        return;
    }

    qWarning() << "Failed to change ranges for plot " << name << ": missing parent group " << groupName;
}

void Plotter::setXRangeGroup(const QString& groupName, std::pair<int64_t, int64_t> range) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setXRanges(range);
        return;
    }

    qWarning() << "Failed to change x range for group " << groupName << ": it's missing";
}

void Plotter::setYRangeGroup(const QString& groupName, std::pair<int64_t, int64_t> range) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setYRanges(range);
        return;
    }

    qWarning() << "Failed to change y range for group " << groupName << ": it's missing";
}

void Plotter::setRangesGroup(const QString& groupName, std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setRanges(xRange, yRange);
        return;
    }

    qWarning() << "Failed to change y range for group " << groupName << ": it's missing";
}

void Plotter::setXRanges(std::pair<int64_t, int64_t> range) {
    for(PlotGroup* group : _plotGroups) {
        group->setXRanges(range);
    }
}

void Plotter::setYRanges(std::pair<int64_t, int64_t> range) {
    for(PlotGroup* group : _plotGroups) {
        group->setYRanges(range);
    }
}

void Plotter::setRanges(std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange) {
    for(PlotGroup* group : _plotGroups) {
        group->setXRanges(xRange);
        group->setYRanges(yRange);
    }
}

void Plotter::setActiveScatterGraph(const QString& groupName, const QString& plotName, const QString& name, bool active) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setActiveScatterGraph(plotName, name, active);
        return;
    }
    qWarning() << "Failed to set active for group " << groupName << ", plot " << plotName << ", graph " << name << ": group missing";
}

void Plotter::setActiveScatterPlot(const QString& groupName, const QString& plotName, bool active) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setActiveScatterPlot(plotName, active);
        return;
    }
    qWarning() << "Failed to set active for group " << groupName << " and plot " << plotName << ": group missing";
}

void Plotter::setActiveScatterGroup(const QString& groupName, bool active) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->setActiveScatterGroup(active);
        return;
    }
    qWarning() << "Failed to set active for group " << groupName << ": group missing";
}

void Plotter::setActiveScatterPlotter(bool active) {
    for(PlotGroup* group : _plotGroups) {
        group->setActiveScatterGroup(active);
    }
}

void Plotter::showModeAreas(const QString& groupName, const QString& plotName, const QVector<double>& values, const QVector<double>& times) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->showModeAreas(plotName, values, times);
        return;
    }
    qWarning() << "Failed to show mode areas for group " << groupName << ", plot " << plotName << ": group missing";
}
