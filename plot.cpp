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

void Plot::setupTooltip() {
    _tooltipEnabled = true;

    _tooltipTracer = new QCPItemTracer(_plot);
    _tooltipTracer->setStyle(QCPItemTracer::tsCircle);
    _tooltipTracer->setSize(10);
    _tooltipTracer->setPen(QPen(Qt::red, 2));
    _tooltipTracer->setBrush(QBrush(Qt::white));
    _tooltipTracer->setVisible(false);

    _tooltipText = new QCPItemText(_plot);
    _tooltipText->setPositionAlignment(Qt::AlignLeft|Qt::AlignTop);
    _tooltipText->setPadding(QMargins(5, 5, 5, 5));
    _tooltipText->setBrush(QBrush(QColor(255, 255, 220, 230)));
    _tooltipText->setPen(QPen(Qt::black));
    _tooltipText->setFont(QFont("Arial", 10));
    _tooltipText->setVisible(false);
    _tooltipText->setLayer("overlay");

    _tooltipRect = new QCPItemRect(_plot);
    _tooltipRect->topLeft->setParentAnchor(_tooltipText->topLeft);
    _tooltipRect->bottomRight->setParentAnchor(_tooltipText->bottomRight);
    _tooltipRect->setBrush(QBrush(QColor(255, 255, 220, 230)));
    _tooltipRect->setPen(QPen(Qt::gray));
    _tooltipRect->setVisible(false);

    _tooltipVLine = new QCPItemStraightLine(_plot);
    _tooltipVLine->setPen(QPen(QColor(100, 100, 100, 150), 1, Qt::DashLine));
    _tooltipVLine->setVisible(false);

    _tooltipHLine = new QCPItemStraightLine(_plot);
    _tooltipHLine->setPen(QPen(QColor(100, 100, 100, 150), 1, Qt::DashLine));
    _tooltipHLine->setVisible(false);
}

void Plot::updateTooltip(double mouseX, double mouseY) {
    double foundX, foundY;
    QCPGraph* nearestGraph = findNearestGraph(mouseX, mouseY, foundX, foundY, 0.1);

    if (nearestGraph) {
        _tooltipTracer->setGraph(nearestGraph);
        _tooltipTracer->setGraphKey(foundX);
        _tooltipTracer->setVisible(true);

        _tooltipVLine->point1->setCoords(foundX, _plot->yAxis->range().lower);
        _tooltipVLine->point2->setCoords(foundX, _plot->yAxis->range().upper);
        _tooltipVLine->setVisible(true);

        _tooltipHLine->point1->setCoords(_plot->xAxis->range().lower, foundY);
        _tooltipHLine->point2->setCoords(_plot->xAxis->range().upper, foundY);
        _tooltipHLine->setVisible(true);

        QString tooltipText;
        tooltipText += QString("X: %1\n").arg(foundX, 0, 'f', 3);
        tooltipText += QString("Y: %1\n").arg(foundY, 0, 'f', 3);
        if (!nearestGraph->name().isEmpty()) {
            tooltipText += QString("Graph: %1").arg(nearestGraph->name());
        }

        _tooltipText->setText(tooltipText);

        double labelX, labelY;

        double xCenter = (_plot->xAxis->range().lower + _plot->xAxis->range().upper) / 2;
        double yCenter = (_plot->yAxis->range().lower + _plot->yAxis->range().upper) / 2;

        double xOffset = (_plot->xAxis->range().upper - _plot->xAxis->range().lower) * 0.05;
        double yOffset = (_plot->yAxis->range().upper - _plot->yAxis->range().lower) * 0.05;

        if (foundX > xCenter) {
            labelX = foundX - xOffset;
            _tooltipText->setPositionAlignment(Qt::AlignRight|Qt::AlignVCenter);
        } else {
            labelX = foundX + xOffset;
            _tooltipText->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        }

        if (foundY > yCenter) {
            labelY = foundY - yOffset;
            _tooltipText->setPositionAlignment(_tooltipText->positionAlignment() | Qt::AlignBottom);
        } else {
            labelY = foundY + yOffset;
            _tooltipText->setPositionAlignment(_tooltipText->positionAlignment() | Qt::AlignTop);
        }

        _tooltipText->position->setCoords(labelX, labelY);
        _tooltipText->setVisible(true);
        _tooltipRect->setVisible(true);

        _plot->replot();
    } else {
        hideTooltip();
    }
}

void Plot::hideTooltip() {
    if (_tooltipTracer) _tooltipTracer->setVisible(false);
    if (_tooltipText) _tooltipText->setVisible(false);
    if (_tooltipRect) _tooltipRect->setVisible(false);
    if (_tooltipVLine) _tooltipVLine->setVisible(false);
    if (_tooltipHLine) _tooltipHLine->setVisible(false);

    if (_plot) _plot->replot();
}

QCPGraph* Plot::findNearestGraph(double mouseX, double mouseY, double& foundX, double& foundY, double tolerance) {
    QCPGraph* nearestGraph = nullptr;
    double minPixelDistance = 15.0;
    double foundPixelDistance = minPixelDistance;

    for (auto it = _graphs.begin(); it != _graphs.end(); ++it) {
        QCPGraph* graph = it.value();
        auto data = graph->data();

        if (data->isEmpty()) continue;

        for (auto dataIt = data->begin(); dataIt != data->end(); ++dataIt) {
            double pointX = dataIt->key;
            double pointY = dataIt->value;

            int pointPixelX = _plot->xAxis->coordToPixel(pointX);
            int pointPixelY = _plot->yAxis->coordToPixel(pointY);

            int mousePixelX = _plot->xAxis->coordToPixel(mouseX);
            int mousePixelY = _plot->yAxis->coordToPixel(mouseY);

            double pixelDistance = std::hypot(pointPixelX - mousePixelX,
                                              pointPixelY - mousePixelY);

            if (pixelDistance < foundPixelDistance) {
                foundPixelDistance = pixelDistance;
                nearestGraph = graph;
                foundX = pointX;
                foundY = pointY;
            }
        }
    }

    return nearestGraph;
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

bool Plot::eventFilter(QObject* watched, QEvent* event) {
    if (watched == _plot && event->type() == QEvent::Leave) {
        hideTooltip();
        return true;
    }

    if (watched == _plot && (event->type() == QEvent::MouseButtonPress ||
                             event->type() == QEvent::Wheel)) {
        hideTooltip();
    }

    return QObject::eventFilter(watched, event);
}

Plot::Plot(const QString& name,
           const QString& horzAxisName, const QString& vertAxisName,
           std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange, QObject *parent)
    : QObject{parent}
    , _plot(new QCustomPlot(qobject_cast<QWidget*>(parent)))
{
    setupTooltip();
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
    connect(_plot, &QCustomPlot::mouseMove, this, [this](QMouseEvent* event){
        double mouseX = _plot->xAxis->pixelToCoord(event->pos().x());
        double mouseY = _plot->yAxis->pixelToCoord(event->pos().y());

        updateTooltip(mouseX, mouseY);
    });

    _plot->installEventFilter(this);
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
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 6));
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
