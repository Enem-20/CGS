#include "plot.h"

#include "qcustomplot.h"

Plot::Plot(const QString& name,
           const QString& horzAxisName, const QString& vertAxisName,
           std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange, QObject *parent)
    : QObject{parent}
    , _plot(new QCustomPlot(qobject_cast<QWidget*>(parent)))
{
    //_plot->plotLayout()->addElement(0,0, new QCPTextElement(_plot, name, QFont("sans", 12, QFont::Bold)));
    _plot->xAxis->setLabel(horzAxisName);
    _plot->yAxis->setLabel(vertAxisName);
    _plot->xAxis->setRange(xRange.first, xRange.second);
    _plot->yAxis->setRange(yRange.first, yRange.second);
    _plot->replot(QCustomPlot::rpQueuedReplot);
    _plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

Plot::~Plot() {
    delete _plot;
    _graphs.clear();
}

QCustomPlot* Plot::getRaw() {
    return _plot;
}

QCPGraph* Plot::getGraph(const QString& name) {
    auto graphIt = _graphs.find(name);
    if(graphIt != _graphs.end()) {
        return graphIt.value();
    }
    qWarning() << "Graph " << name << " missing";
    return nullptr;
}

QCPGraph* Plot::createGraph(const QString& name) {
    if(!_graphs.contains(name)) {
        QCPGraph* graph = _plot->addGraph();
        graph->setName(name);
        _graphs.emplace(name, graph);
        return graph;
    }
    qWarning() << "Graph " << name << " already exists. An old graph with same name will be returned";
    return _graphs.find(name).value();
}

void Plot::removeGraph(const QString& name) {
    if(_graphs.contains(name)) {
        _plot->removeGraph(getGraph(name));
        _graphs.remove(name);
        return;
    }
    qWarning() << "Can't remove graph " << name << ": it's missing";
}

void Plot::clear() {
    _plot->clearGraphs();
    _graphs.clear();
}

void Plot::replot() {
    _plot->replot(QCustomPlot::rpQueuedReplot);
}

void Plot::setData(const QString& graphName,
             const QVector<double>& keys, const QVector<double>& values) {
    auto graphIt = _graphs.find(graphName);
    if(graphIt != _graphs.end()) {
        graphIt.value()->setData(keys, values);
        return;
    }
    qWarning() << "Failed to set data for " << graphName << " it's missing";
}
void Plot::addData(const QString& graphName,
             const QVector<double>& keys, const QVector<double>& values) {
    auto graphIt = _graphs.find(graphName);
    if(graphIt != _graphs.end()) {
        graphIt.value()->addData(keys, values);
        return;
    }
    qWarning() << "Failed to add data for " << graphName << " it's missing";
}
void Plot::addPoint(const QString& graphName,
              const QPair<double, double>& point) {
    auto graphIt = _graphs.find(graphName);
    if(graphIt != _graphs.end()) {
        graphIt.value()->addData({point.first}, {point.second});
        return;
    }
    qWarning() << "Failed to add point for " << graphName << " it's missing";
}
