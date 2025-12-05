#ifndef PLOT_H
#define PLOT_H

#include <QObject>
#include <QHash>

class QCustomPlot;
class QCPGraph;

class Plot : public QObject
{
    Q_OBJECT
private:
    QHash<QString, QCPGraph*> _graphs;
    QCustomPlot* _plot;
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
};

#endif // PLOT_H
