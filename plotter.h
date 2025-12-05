#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QHash>

namespace Ui {
class Plotter;
}
class QResizeEvent;

class QCustomPlot;
class QCPGraph;
class PlotGroup;
class Plot;


class Plotter : public QWidget
{
    Q_OBJECT
private:
    QHash<QString, PlotGroup*> _plotGroups;
    Ui::Plotter *ui;
public:
    explicit Plotter(QWidget *parent = nullptr);
    ~Plotter();
protected:
    void resizeEvent(QResizeEvent* event) override;
public slots:
    QCPGraph* getGraph(const QString& groupName, const QString& plotName, const QString& name);
    Plot* getPlot(const QString& groupName, const QString& name);
    PlotGroup* getGroup(const QString& name);
    void removeGraph(const QString& groupName, const QString& plotName, const QString& name);
    void removePlot(const QString& groupName, const QString& name);
    void removeGroup(const QString& name);
    void clear();

    PlotGroup* createPlotGroup(const QString& name);
    Plot* createPlot(const QString& groupName, const QString& name,
                    const QString& horzAxisName, const QString& vertAxisName,
                    std::pair<int64_t, int64_t> xRange = {0,0}, std::pair<int64_t, int64_t> yRange = {0,0});
    QCPGraph* createGraph(const QString& groupName, const QString& plotName, const QString& name);

    void setData(const QString& groupName, const QString& plotName, const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addData(const QString& groupName, const QString& plotName, const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addPoint(const QString& groupName, const QString& plotName, const QString& graphName,
                  const QPair<double, double>& point);
};

#endif // PLOTTER_H
