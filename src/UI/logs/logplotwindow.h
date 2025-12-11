#ifndef LOGPLOTWINDOW_H
#define LOGPLOTWINDOW_H

#include <QWidget>
#include <QTreeWidget>

#include "UI/plotter/plotter.h"

#include "mavlink/dataflashparser.h"
#include "logparameterswindow.h"

namespace Ui {
class LogPlotWindow;
}

class LogPlotWindow : public QWidget {
    Q_OBJECT

    struct ActiveGraphHandle {
        qsizetype groupIndex = 0;
        qsizetype paramIndex = 0;
        qsizetype timeIndex = 0;
        QTreeWidgetItem* treeItem = nullptr;
    };

public:
    explicit LogPlotWindow(QWidget *parent = nullptr);
    ~LogPlotWindow();

    static void openFileToReview(const QString& title = "");

private:
    Ui::LogPlotWindow *ui;
    Plotter* _plotter;
    DataFlashParser _parser;
    QList<ActiveGraphHandle> _activeGraphs;
    QVector<QVector<QPair<double, double>>> _rangesCacheX;
    QVector<QVector<QPair<double, double>>> _rangesCacheY;

    void parseFile(const QString& path);
    void resetXRange();
    void resetYRange();

    QPair<double, double> getRange(const QVector<double>& values, double padding = 0.05);

public slots:
    void wrapShow();
private slots:
    void on_resetViewButton_clicked();
    void on_resetPlotButton_clicked();
    void handleDataSelectionChanged(QTreeWidgetItem *item, int column);
    void on_showParamsButton_clicked();
};

#endif // LOGPLOTWINDOW_H
