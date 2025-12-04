#ifndef LOGPLOTWINDOW_H
#define LOGPLOTWINDOW_H

#include <QWidget>
#include <QTreeWidget>

#include "plotter.h"

#include "dataflashparser.h"

namespace Ui {
class LogPlotWindow;
}

class LogPlotWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LogPlotWindow(QWidget *parent = nullptr);
    ~LogPlotWindow();

private:
    Ui::LogPlotWindow *ui;
    Plotter* _plotter;
    DataFlashParser _parser;

public slots:
    void wrapShow();
private slots:
    void on_resetViewButton_clicked();
    void on_resetPlotButton_clicked();
    void handleDataSelectionChanged(QTreeWidgetItem *item, int column);
};

#endif // LOGPLOTWINDOW_H
