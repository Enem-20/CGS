#ifndef LOGPLOTWINDOW_H
#define LOGPLOTWINDOW_H

#include <QWidget>
#include <QTreeWidget>

#include "plotter.h"

#include "dataflashparser.h"
#include "logparameterswindow.h"

namespace Ui {
class LogPlotWindow;
}

class LogPlotWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LogPlotWindow(QWidget *parent = nullptr);
    ~LogPlotWindow();

    static void showFileContents(const QString& filePath, const QString& title = "");

private:
    Ui::LogPlotWindow *ui;
    Plotter* _plotter;
    DataFlashParser _parser;

    void parseFile(const QString& path);

public slots:
    void wrapShow();
private slots:
    void on_resetViewButton_clicked();
    void on_resetPlotButton_clicked();
    void handleDataSelectionChanged(QTreeWidgetItem *item, int column);
    void on_showParamsButton_clicked();
};

#endif // LOGPLOTWINDOW_H
