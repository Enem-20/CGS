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

    _plotter->createPlotGroup("LogReview");

    _plotter->createPlot("LogReview", "FLME", "time", "value", {0, 120}, {-100, 100});
    _plotter->createGraph("LogReview", "FLME", "azim");

    connect(ui->dataTree, &QTreeWidget::itemChanged, this, &LogPlotWindow::handleDataSelectionChanged);
}

LogPlotWindow::~LogPlotWindow()
{
    delete ui;
}

void LogPlotWindow::wrapShow() {
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString path = QFileDialog::getOpenFileName(nullptr, "Select log file to review", downloadsPath);
    if (path == "") return;

    _parser.parseFile(path);

    const QList<LogFormatData>& data = _parser.getData();

    ui->dataTree->clear();
    for (qsizetype groupIndex = 0; groupIndex < data.size(); groupIndex++) {
        if (!data[groupIndex].columns.contains("TimeUS")) {
            continue;
        }

        QTreeWidgetItem* topItem = new QTreeWidgetItem();
        topItem->setText(0, data[groupIndex].name);

        for (qsizetype columnIndex = 0; columnIndex < data[groupIndex].columns.size(); columnIndex++) {
            if (data[groupIndex].columns[columnIndex] == "TimeUS") continue;
            if (data[groupIndex].values[columnIndex].size() == 0) continue;

            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, data[groupIndex].columns[columnIndex]);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(0, Qt::Unchecked);
            topItem->addChild(item);
        }

        if (topItem->childCount() == 0) {
            delete topItem;
            continue;
        }

        ui->dataTree->addTopLevelItem(topItem);
    }

    _plotter->addData("LogReview", "FLME", "azim", data[184].values[0], data[184].values[2]);

    show();
}

void LogPlotWindow::on_resetViewButton_clicked() {
    _plotter->adjustSize();
}


void LogPlotWindow::on_resetPlotButton_clicked() {

}


void LogPlotWindow::handleDataSelectionChanged(QTreeWidgetItem *item, int column) {
    qDebug() << "Item changed";

    QString label = item->text(column);
    QString group = item->parent()->text(column);

    const QList<LogFormatData>& data = _parser.getData();

    for (qsizetype i = 0; i < data.size(); i++) {
        if (data[i].name != group) continue;
        for (qsizetype j = 0; j < data[i].columns.size(); j++) {
            if (data[i].columns[j] != label) continue;

            if (item->checkState(column)) {
                qsizetype timeIndex = 0;
                for (qsizetype k = 0; k < data[i].columns.size(); k++) {
                    if (data[i].columns[k] == "TimeUS") {
                        timeIndex = k;
                        break;
                    }
                }

                _plotter->createGraph("LogReview", "FLME", label);
                _plotter->setData("LogReview", "FLME", label, data[i].values[timeIndex], data[i].values[j]);
            }
            else {
                _plotter->setData("LogReview", "FLME", label, {}, {});
            }

            break;
        }
        break;
    }
}
