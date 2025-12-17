#ifndef TELEMETRYWINDOW_H
#define TELEMETRYWINDOW_H

#include <QWidget>
#include <QTreeWidget>
#include <QVector>

#include "UI/plotter/plotgroup.h"
#include "UI/plotter/plot.h"

namespace Ui {
class TelemetryWindow;
}

class Plotter;

struct TelemetryParam {
    QString name;
    QVector<float> values;
    QTreeWidgetItem* treeItem;
    Plot* plot;
};

struct TelemetryGroup {
    quint64 timestamp;
    QString groupName;
    QHash<QString, TelemetryParam> _params;
    QTreeWidgetItem* treeItem;
    PlotGroup* plotGroup;
    QVector<float> timeValues;
};

class TelemetryWindow : public QWidget {
    Q_OBJECT
    
private:
    Plotter* _plotter;
    QHash<QString, TelemetryGroup> _telemetryMap;
    QString _customTelemetry = "^(\\d+):\\s*([^:]+?)\\s*:\\s*([^:]+?)\\s*:\\s*([-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?)";
    Ui::TelemetryWindow *ui;
    double _timeRangeMillis = 30000.0;
    double _rangeYPadding = 0.05;

private:
    bool groupExists(const QString& groupName);
    bool parameterExists(const QString& groupName, const QString& paramName);
    bool parameterExists(const TelemetryGroup& group, const QString& paramName);
    TelemetryGroup& createGroup(const QString& name, uint64_t timestamp);
    void createParam(TelemetryGroup& group, const QString& name, bool active = false);
    void plotValue(const QString& groupName, const QString& paramName, double t, double value);
    void handleTelemetrySelectionChanged(QTreeWidgetItem *item, int column);

public:
    explicit TelemetryWindow(QWidget *parent = nullptr);
    ~TelemetryWindow();

signals:
    void updated(const QStringList& telemetryQueue);

public slots:
    void onVehicleOrientationUpdated(const QVector3D& orientation, uint64_t timestamp);
    void onVehicleVelocityUpdated(const QVector3D& velocity, uint64_t timestamp);
    void onVehicleAltitudeUpdated(float altitude, uint64_t timestamp);
    void onStatusMessageReceived(const QString& text, const QString& severity);
};

#endif // TELEMETRYWINDOW_H
