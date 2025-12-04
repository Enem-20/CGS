#include "logplotwindow.h"
#include "ui_logplotwindow.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QSplitter>

LogPlotWindow::LogPlotWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogPlotWindow)
{
    ui->setupUi(this);

    QSplitter* splitter = new QSplitter(ui->frameMain);
    ui->frameMain->layout()->addWidget(splitter);

    _plotter = new Plotter(splitter);
    // splitter->addWidget(_plotter);

    ui->dataList->setParent(splitter);
    // splitter->addWidget(ui->dataList);

    _plotter->createPlotGroup("FLME");
    _plotter->createPlot("FLME", "azim", "time", "azim", {0, 120}, {-100, 100});
    _plotter->createGraph("FLME", "azim", "azim");

}

LogPlotWindow::~LogPlotWindow()
{
    delete ui;
}

void LogPlotWindow::wrapShow() {
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString path = QFileDialog::getOpenFileName(nullptr, "Select log file to review", downloadsPath);
    if (path == "") return;

    DataFlashParser parser;
    parser.parseFile(path);

    const QList<LogFormatData>& data = parser.getData();

    _plotter->addData("FLME", "azim", "azim", data[184].values[0], data[184].values[1]);

    show();
}

void LogPlotWindow::on_resetViewButton_clicked() {
    _plotter->adjustSize();
}


void LogPlotWindow::on_resetPlotButton_clicked() {

}

