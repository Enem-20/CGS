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
    void setName(const QString& name);
    void createPlot(const QString& name,
                    const QString& horzAxisName, const QString& vertAxisName,
                    std::pair<int64_t, int64_t> xRange = {0,0}, std::pair<int64_t, int64_t> yRange = {0,0});
    void createGraph(const QString& plotName, const QString& name);

    void setData(const QString& plotName, const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addData(const QString& plotName, const QString& graphName,
                 const QVector<double>& keys, const QVector<double>& values);
    void addPoint(const QString& plotName, const QString& graphName,
                  const QPair<double, double>& point);
};

#endif // PLOTGROUP_H
