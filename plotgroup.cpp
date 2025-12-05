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
    clear();
    delete ui;
}

void PlotGroup::resizeEvent(QResizeEvent* resizeEvent) {
    QWidget::resizeEvent(resizeEvent);

    for(auto plot : _plots.values()) {
        plot->getRaw()->replot(QCustomPlot::rpQueuedReplot);
        plot->getRaw()->updateGeometry();
    }
}

QCPGraph* PlotGroup::getGraph(const QString& plotName, const QString& name) {
    auto plotIt = _plots.find(plotName);
    if(plotIt != _plots.end()) {
        return plotIt.value()->getGraph(name);
    }
    qWarning() << "Graph " << name << " missing due to missing parent plot " << plotName;
    return nullptr;
}

Plot* PlotGroup::getPlot(const QString& name) {
    auto plotIt = _plots.find(name);
    if(plotIt != _plots.end())
        return plotIt.value();
    qWarning() << "Plot " << name << " missing";
    return nullptr;
}

void PlotGroup::removeGraph(const QString& plotName, const QString& name) {
    auto plotIt = _plots.find(plotName);
    if(plotIt != _plots.end()) {
        plotIt.value()->removeGraph(name);
        return;
    }
    qWarning() << "Can't remove graph " << name << " due to missing plot " << plotName;
}

void PlotGroup::removePlot(const QString& name) {
    auto plotIt = _plots.find(name);
    if(plotIt != _plots.end()) {
        plotIt.value()->deleteLater();
        _plots.remove(name);
        return;
    }
    qWarning() << "Can't remove plot " << name << ": it's missing";
}

void PlotGroup::clear() {
    for(Plot* plot : _plots) {
        plot->deleteLater();
    }
    _plots.clear();
}

void PlotGroup::setName(const QString& name) {
    ui->groupName->setText(name);
}

Plot* PlotGroup::createPlot(const QString& name,
                         const QString& horzAxisName, const QString& vertAxisName,
                         std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange) {
    auto plotIt = _plots.find(name);
    if(plotIt == _plots.end()) {
        Plot* plot = new Plot(name, horzAxisName, vertAxisName, xRange, yRange, ui->plots);

        _plots.emplace(name, plot);
        ui->plots->layout()->addWidget(plot->getRaw());
        return plot;
    }
    qWarning() << "Plot " << name << " already exists. Returned old one with same name";

    return plotIt.value();
}

QCPGraph* PlotGroup::createGraph(const QString& plotName, const QString& name) {
    auto plotIt = _plots.find(plotName);
    if(plotIt != _plots.end()) {
        QCPGraph* graph = plotIt.value()->createGraph(name);
        return graph;
    }
    qWarning() << "Graph " << name << " wasn't create. Plot " << plotName << " is missing";

    return nullptr;
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
