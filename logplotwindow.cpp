#include "logplotwindow.h"
#include "ui_logplotwindow.h"

LogPlotWindow::LogPlotWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogPlotWindow)
{
    ui->setupUi(this);
}

LogPlotWindow::~LogPlotWindow()
{
    delete ui;
}
