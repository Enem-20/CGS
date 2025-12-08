#ifndef PLOT_H
#define PLOT_H

#include <QObject>
#include <QHash>
#include <QSet>
#include <QColor>

class QCustomPlot;
class QCPGraph;
class QCPItemTracer;
class QCPItemText;
class QCPItemRect;
class QCPItemStraightLine;

inline size_t qHash(const QColor &color, size_t seed = 0) noexcept
{
    return qHashMulti(seed, color.red(), color.green(), color.blue(), color.alpha());
}

class Plot : public QObject
{
    Q_OBJECT
private:
    QCPItemTracer* _tooltipTracer = nullptr;
    QCPItemText* _tooltipText = nullptr;
    QCPItemRect* _tooltipRect = nullptr;
    QCPItemStraightLine* _tooltipVLine = nullptr;
    QCPItemStraightLine* _tooltipHLine = nullptr;
    bool _tooltipEnabled = false;

    QSet<QColor> _usedColors;
    QHash<QString, QCPGraph*> _graphs;
    QCustomPlot* _plot;
private:
    void setupTooltip();
    void updateTooltip(double mouseX, double mouseY);
    void hideTooltip();
    QCPGraph* findNearestGraph(double mouseX, double mouseY, double& foundX, double& foundY, double tolerance = 0.1);

    bool colorContrastedWithOthers(const QColor& color);
    QColor generateColor();
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
public:
    explicit Plot(const QString& name,
                  const QString& horzAxisName, const QString& vertAxisName,
                  std::pair<int64_t, int64_t> xRange = {0,0}, std::pair<int64_t, int64_t> yRange = {0,0}, QObject *parent = nullptr);
    ~Plot();
signals:
public slots:
    QCustomPlot* getRaw();
    QCPGraph* getGraph(const QString& name);
    QCPGraph* createGraph(const QString& name);
    void removeGraph(const QString& name);
    void clear();
    void replot();

    void setData(const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addData(const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addPoint(const QString& graphName,
                  const QPair<double, double>& point);
    void setXRange(std::pair<int64_t, int64_t> range);
    void setXRange(std::pair<double, double> range);
    void setYRange(std::pair<int64_t, int64_t> range);
    void setYRange(std::pair<double, double> range);
    void setRanges(std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange);
    void setRanges(std::pair<double, double> xRange, std::pair<double, double> yRange);

    void setActiveScatterGraph(const QString& name, bool active);
    void setActiveScatterPlot(bool active);
};

#endif // PLOT_H
