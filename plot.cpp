#include "plot.h"

#include <random>

#include "qcustomplot.h"

double hsvDistance(const QColor& c1, const QColor& c2) {
    QColor hsv1 = c1.toHsv();
    QColor hsv2 = c2.toHsv();

    double h1 = hsv1.hueF();  // 0-1
    double h2 = hsv2.hueF();
    double hDiff = fabs(h1 - h2);
    hDiff = qMin(hDiff, 1.0 - hDiff);

    double sDiff = fabs(hsv1.saturationF() - hsv2.saturationF());
    double vDiff = fabs(hsv1.valueF() - hsv2.valueF());

    return sqrt(0.5 * pow(hDiff, 2) + 0.25 * pow(sDiff, 2) + 0.25 * pow(vDiff, 2));
}

bool areColorsDistinguishableHSV(const QColor& c1, const QColor& c2,
                                 double threshold = 0.15) {
    return hsvDistance(c1, c2) > threshold;
}

bool Plot::colorContrastedWithOthers(const QColor& color) {
    bool result = true;
    for(QColor cmp : _usedColors.values()) {
        result = result && areColorsDistinguishableHSV(cmp, color);
    }
    return result;
}

QColor Plot::generateColor() {
    QColor color(255, 0, 0);

    while(_usedColors.contains(color) && !colorContrastedWithOthers(color)) {
        QCPColorGradient gradient(QCPColorGradient::gpSpectrum);
        std::random_device rd;
        std::default_random_engine re(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        color = gradient.color(dist(re), QCPRange(0, 1));
    }
    _usedColors.insert(color);
    return color;
}

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
    _plot->legend->setVisible(true);
    _plot->autoAddPlottableToLegend();
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
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDot, 1));
        graph->setPen(generateColor());
        //graph->addToLegend();
        graph->setName(name);
        _graphs.emplace(name, graph);
        return graph;
    }
    qWarning() << "Graph " << name << " already exists. An old graph with same name will be returned";
    return _graphs.find(name).value();
}

void Plot::removeGraph(const QString& name) {
    if(_graphs.contains(name)) {
        QCPGraph* graph = getGraph(name);
        _usedColors.remove(graph->pen().color());
        _plot->removeGraph(graph);
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
