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

LogPlotWindow::~LogPlotWindow() {
    delete ui;
}

void LogPlotWindow::showFileContents(const QString& filePath, const QString& title) {
    LogPlotWindow* tempWindow = new LogPlotWindow();
    if (title != "") {
        tempWindow->setWindowTitle(title);
    }
    tempWindow->setAttribute(Qt::WA_DeleteOnClose);
    tempWindow->parseFile(filePath);
    tempWindow->show();
}

void LogPlotWindow::wrapShow() {
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString path = QFileDialog::getOpenFileName(nullptr, "Select log file to review", downloadsPath);

    parseFile(path);

    show();
}

void LogPlotWindow::parseFile(const QString& path) {
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
}

void LogPlotWindow::resetXRange() {
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();

    const QList<LogFormatData>& data = _parser.getData();

    for (const ActiveGraphHandle& handle : _activeGraphs) {
        QVector<double> values = data[handle.groupIndex].values[handle.timeIndex];
        std::sort(values.begin(), values.end());
        min = std::min(min, values[0]);
        max = std::max(max, values.back());
    }

    min *= (1.0 / 1.05);
    max *= 1.05;

    if (min >= max) {
        min = 0.0;
        max = 1.0;
    }

    _plotter->setXRanges({min, max});
}

void LogPlotWindow::resetYRange() {
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();

    const QList<LogFormatData>& data = _parser.getData();

    for (const ActiveGraphHandle& handle : _activeGraphs) {
        QVector<double> values = data[handle.groupIndex].values[handle.paramIndex];
        std::sort(values.begin(), values.end());
        min = std::min(min, values[0]);
        max = std::max(max, values.back());
    }

    min *= (1.0 / 1.05);
    max *= 1.05;

    min = std::max(min, -100000.0);
    max = std::min(max, 100000.0);

    if (min >= max) {
        min = -1.0;
        max = 1.0;
    }

    _plotter->setYRanges({min, max});
}

void LogPlotWindow::on_resetViewButton_clicked() {
    resetXRange();
    resetYRange();
}

void LogPlotWindow::on_resetPlotButton_clicked() {
    for (const ActiveGraphHandle& handle : _activeGraphs) {
        handle.treeItem->setCheckState(0, Qt::Unchecked);
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

                _activeGraphs.push_back(ActiveGraphHandle{i, j, timeIndex, item});

                _plotter->createGraph("LogReview", "FLME", label);
                _plotter->setData("LogReview", "FLME", label, data[i].values[timeIndex], data[i].values[j]);
            }
            else {
                _activeGraphs.removeIf([item](const ActiveGraphHandle& handle){
                    return handle.treeItem ==  item;
                });
                _plotter->removeGraph("LogReview", "FLME", label);
            }

            break;
        }
        break;
    }

    _plotter->replot();
}
