#ifndef LOGPLOTWINDOW_H
#define LOGPLOTWINDOW_H

#include <QWidget>

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
public slots:
    void wrapShow();
private slots:
    void on_resetViewButton_clicked();
    void on_resetPlotButton_clicked();
};

#endif // LOGPLOTWINDOW_H
