#include "plotgroup.h"
#include "ui_plotgroup.h"

#include "plot.h"
#include "qcustomplot.h"

PlotGroup::PlotGroup(const QString& name, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlotGroup)
{
    ui->setupUi(this);
    setName(name);
    setAttribute(Qt::WA_PendingResizeEvent);
}

PlotGroup::~PlotGroup()
{
    delete ui;
}

void PlotGroup::resizeEvent(QResizeEvent* resizeEvent) {
    QWidget::resizeEvent(resizeEvent);

    for(auto plot : _plots.values()) {
        plot->getRaw()->replot(QCustomPlot::rpQueuedReplot);
        plot->getRaw()->updateGeometry();
    }
}

void PlotGroup::setName(const QString& name) {
    ui->groupName->setText(name);
}

void PlotGroup::createPlot(const QString& name,
                         const QString& horzAxisName, const QString& vertAxisName,
                         std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange) {
    Plot* plot = new Plot(name, horzAxisName, vertAxisName, xRange, yRange, ui->plots);
    _plots.emplace(name, plot);
    ui->plots->layout()->addWidget(plot->getRaw());
}

void PlotGroup::createGraph(const QString& plotName, const QString& name) {
    auto plotIt = _plots.find(plotName);
    if(plotIt != _plots.end()) {
        QCPGraph* graph = plotIt.value()->createGraph(name);
        return;
    }
    qWarning() << "Graph " << name << " wasn't create. Plot " << plotName << " is missing";
}

void PlotGroup::setData(const QString& plotName, const QString& graphName,
                        const QVector<double>& keys, const QVector<double>& values) {
    auto plotIt = _plots.find(plotName);
    if(plotIt != _plots.end()) {
        plotIt.value()->setData(graphName, keys, values);
        return;
    }
    qWarning() << "Failed to set data for plot " << plotName << " it's missing";
}
void PlotGroup::addData(const QString& plotName, const QString& graphName,
                        const QVector<double>& keys, const QVector<double>& values) {
    auto plotIt = _plots.find(plotName);
    if(plotIt != _plots.end()) {
        plotIt.value()->addData(graphName, keys, values);
        return;
    }
    qWarning() << "Failed to add data for plot " << plotName << " it's missing";
}
void PlotGroup::addPoint(const QString& plotName, const QString& graphName,
                         const QPair<double, double>& point) {
    auto plotIt = _plots.find(plotName);
    if(plotIt != _plots.end()) {
        plotIt.value()->addPoint(graphName, point);
        return;
    }
    qWarning() << "Failed to add point for plot " << plotName << " it's missing";
}
