#include "logplotwindow.h"
#include "ui_logplotwindow.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QSplitter>
#include <QTreeWidgetItem>

LogPlotWindow::LogPlotWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogPlotWindow)
{
    ui->setupUi(this);

    _plotter = new Plotter(ui->plotFrame);
    ui->plotFrame->layout()->addWidget(_plotter);
    _plotter->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

    // ui->plotFrame->setLayout(new QVBoxLayout());

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

    for (qsizetype groupIndex = 0; groupIndex < data.size(); groupIndex++) {
        QTreeWidgetItem* topItem = new QTreeWidgetItem();
        topItem->setText(0, data[groupIndex].name);
        for (qsizetype columIndex = 0; columIndex < data[groupIndex].columns.size(); columIndex++) {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, data[groupIndex].columns[columIndex]);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            topItem->addChild(item);
        }
        ui->dataTree->addTopLevelItem(topItem);
    }

    _plotter->addData("FLME", "azim", "azim", data[184].values[0], data[184].values[1]);

    show();
}

void LogPlotWindow::on_resetViewButton_clicked() {
    _plotter->adjustSize();
}


void LogPlotWindow::on_resetPlotButton_clicked() {

}

