#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QHash>

namespace Ui {
class Plotter;
}

class QCustomPlot;
class QCPGraph;
class PlotGroup;


class Plotter : public QWidget
{
    Q_OBJECT
private:
    QHash<QString, PlotGroup*> _plotGroups;
    Ui::Plotter *ui;
public:
    explicit Plotter(QWidget *parent = nullptr);
    ~Plotter();
public slots:
    void createPlotGroup(const QString& name);
    void createPlot(const QString& groupName, const QString& name,
                    const QString& horzAxisName, const QString& vertAxisName,
                    std::pair<int64_t, int64_t> xRange = {0,0}, std::pair<int64_t, int64_t> yRange = {0,0});
    void createGraph(const QString& groupName, const QString& plotName, const QString& name);

    void setData(const QString& groupName, const QString& plotName, const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addData(const QString& groupName, const QString& plotName, const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addPoint(const QString& groupName, const QString& plotName, const QString& graphName,
                  const QPair<double, double>& point);
};

#endif // PLOTTER_H
