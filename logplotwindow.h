#ifndef LOGPLOTWINDOW_H
#define LOGPLOTWINDOW_H

#include <QWidget>

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
};

#endif // LOGPLOTWINDOW_H
