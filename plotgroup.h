#ifndef PLOTGROUP_H
#define PLOTGROUP_H

#include <QWidget>
#include <QHash>
#include <QResizeEvent>

namespace Ui {
class PlotGroup;
}

class Plot;
class QCPGraph;

class PlotGroup : public QWidget
{
    Q_OBJECT
private:
    QHash<QString, Plot*> _plots;
    Ui::PlotGroup *ui;
public:
    explicit PlotGroup(const QString& name, QWidget *parent = nullptr);
    ~PlotGroup();

protected:
    void resizeEvent(QResizeEvent* resizeEvent);
public slots:
    QCPGraph* getGraph(const QString& plotName, const QString& name);
    Plot* getPlot(const QString& name);
    void removeGraph(const QString& plotName, const QString& name);
    void removePlot(const QString& name);
    void clear();
    void replotExact(const QString& name);
    void replot();

    void setName(const QString& name);
    Plot* createPlot(const QString& name,
                    const QString& horzAxisName, const QString& vertAxisName,
                    std::pair<int64_t, int64_t> xRange = {0,0}, std::pair<int64_t, int64_t> yRange = {0,0});
    QCPGraph* createGraph(const QString& plotName, const QString& name);

    void setData(const QString& plotName, const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addData(const QString& plotName, const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addPoint(const QString& plotName, const QString& graphName,
                  const QPair<double, double>& point);

    void setXRangePlot(const QString& plotName, std::pair<int64_t, int64_t> range);
    void setYRangePlot(const QString& plotName, std::pair<int64_t, int64_t> range);
    void setRangesPlot(const QString& plotName, std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange);
    void setXRanges(std::pair<int64_t, int64_t> range);
    void setYRanges(std::pair<int64_t, int64_t> range);
    void setRanges(std::pair<int64_t, int64_t> xRange, std::pair<int64_t, int64_t> yRange);
};

#endif // PLOTGROUP_H
