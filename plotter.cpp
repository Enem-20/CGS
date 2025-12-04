#include "plotter.h"
#include "ui_plotter.h"

#include "qcustomplot.h"

#include "plotgroup.h"

Plotter::Plotter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Plotter)
{
    ui->setupUi(this);
    ui->plots->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

Plotter::~Plotter()
{
    delete ui;
}

void Plotter::createPlotGroup(const QString& name) {
    auto groupIt = _plotGroups.find(name);
    if(groupIt == _plotGroups.end()) {
        PlotGroup* group = new PlotGroup(name, ui->plots);
        group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        _plotGroups.emplace(name, group);
    } else {
        qWarning() << "Plot group " << name << " already exists";
    }
}

void Plotter::createPlot(const QString& groupName, const QString& name,
                         const QString& horzAxisName, const QString& vertAxisName,
                         std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->createPlot(name, horzAxisName, vertAxisName, xRange, yRange);
    } else {
        createPlotGroup(groupName);
        groupIt = _plotGroups.find(groupName);
        groupIt.value()->createPlot(name, horzAxisName, vertAxisName, xRange, yRange);
    }
}

void Plotter::createGraph(const QString& groupName, const QString& plotName, const QString& name) {
    auto groupIt = _plotGroups.find(groupName);
    if(groupIt != _plotGroups.end()) {
        groupIt.value()->createGraph(plotName, name);
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
