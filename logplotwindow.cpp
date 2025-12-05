#include "logplotwindow.h"
#include "ui_logplotwindow.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QSplitter>
#include <QTreeWidgetItem>
#include <QCheckBox>

LogPlotWindow::LogPlotWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogPlotWindow)
{
    ui->setupUi(this);

    _plotter = new Plotter(ui->plotFrame);
    ui->plotFrame->layout()->addWidget(_plotter);
    _plotter->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

    _plotter->createPlotGroup("LogReview");
    _plotter->createPlot("LogReview", "FLME", "time", "value", {0, 120}, {-100, 100});

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

    show();
}

void LogPlotWindow::on_resetViewButton_clicked() {

}

void LogPlotWindow::on_resetPlotButton_clicked() {
    auto items = ui->dataTree->findItems(".*", Qt::MatchRegularExpression);
    for(QTreeWidgetItem* item : items) {
        for(size_t i = 0; i < item->childCount(); ++i) {
            if(item->child(i)->checkState(0) == Qt::Checked) {
                item->child(i)->setCheckState(0, Qt::Unchecked);
            }
        }
    }
}

void LogPlotWindow::on_showParamsButton_clicked() {
    LogParametersWindow::showTemporary(_parser.getParameters());
}

void LogPlotWindow::handleDataSelectionChanged(QTreeWidgetItem *item, int column) {
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
                _plotter->removeGraph("LogReview", "FLME", label);
            }

            break;
        }
        break;
    }

    _plotter->replot();
}
