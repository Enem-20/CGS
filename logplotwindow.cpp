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
    _plotter->createPlot("LogReview", "LogPlot", "time(s)", "value", {0, 120}, {-100, 100});

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

    _rangesCacheX.resize(data.size());
    _rangesCacheY.resize(data.size());

    ui->dataTree->clear();
    for (qsizetype groupIndex = 0; groupIndex < data.size(); groupIndex++) {
        qsizetype timeIndex = data[groupIndex].columns.indexOf("TimeUS");
        if (timeIndex == -1) {
            continue;
        }

        if (data[groupIndex].values[timeIndex].size() == 0) {
            continue;
        }

        QPair<double, double> rangeX = getRange(data[groupIndex].values[timeIndex]);

        QTreeWidgetItem* topItem = new QTreeWidgetItem();
        topItem->setText(0, data[groupIndex].name);

        _rangesCacheX[groupIndex].resize(data[groupIndex].columns.size());
        _rangesCacheY[groupIndex].resize(data[groupIndex].columns.size());

        for (qsizetype columnIndex = 0; columnIndex < data[groupIndex].columns.size(); columnIndex++) {
            if (columnIndex == timeIndex || data[groupIndex].values[columnIndex].size() == 0) {
                continue;
            }

            _rangesCacheX[groupIndex][columnIndex] = rangeX;
            _rangesCacheY[groupIndex][columnIndex] = getRange(data[groupIndex].values[columnIndex]);

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

    for (const ActiveGraphHandle& handle : _activeGraphs) {
        min = std::min(min, _rangesCacheX[handle.groupIndex][handle.paramIndex].first);
        max = std::max(max, _rangesCacheX[handle.groupIndex][handle.paramIndex].second);
    }

    if (min >= max) {
        min = 0.0;
        max = 1.0;
    }

    _plotter->setXRanges({min, max});
}

void LogPlotWindow::resetYRange() {
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();

    for (const ActiveGraphHandle& handle : _activeGraphs) {
        min = std::min(min, _rangesCacheY[handle.groupIndex][handle.paramIndex].first);
        max = std::max(max, _rangesCacheY[handle.groupIndex][handle.paramIndex].second);
    }

    if (min >= max) {
        min = -1.0;
        max = 1.0;
    }

    _plotter->setYRanges({min, max});
}

QPair<double, double> LogPlotWindow::getRange(const QVector<double>& _values, double _padding) {
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();

    QVector<double> values = _values;
    std::sort(values.begin(), values.end());
    min = std::min(min, values[0]);
    max = std::max(max, values.back());

    double range = max - min;
    double padding = range * _padding;

    min -= padding;
    max += padding;

    if (min >= max) {
        min = -1.0;
        max = 1.0;
    }

    return QPair<double, double>{min, max};
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

                _plotter->createGraph("LogReview", "LogPlot", label);
                _plotter->setData("LogReview", "LogPlot", label, data[i].values[timeIndex], data[i].values[j]);
            }
            else {
                _activeGraphs.removeIf([item](const ActiveGraphHandle& handle){
                    return handle.treeItem ==  item;
                });
                _plotter->removeGraph("LogReview", "LogPlot", label);
            }

            break;
        }
        break;
    }

    _plotter->replot();
}
