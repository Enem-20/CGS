#include "logswindow.h"
#include "ui_logswindow.h"

#include <QMessageBox>
#include <QStandardPaths>
#include <QFileDialog>

#include "logplotwindow.h"
#include "vehicle/logs/vehiclelogs.h"

LogsWindow::LogsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogsWindow)
{
    ui->setupUi(this);
}

LogsWindow::~LogsWindow() {
    delete ui;
}

void LogsWindow::onActiveDeviceChanged(QStringView deviceName) {
    ui->progressBar->setValue(0);
    ui->recievedBytes->setText(QString("Download stopped"));
}

void LogsWindow::onLogListReceived(const QVector<LogEntry>& entries) {
    ui->tableWidget->setRowCount(0);

    QLocale locale = this->locale();

    for (const LogEntry& entry : entries) {
        QDateTime timestamp;
        timestamp.setSecsSinceEpoch(entry.timestamp);
        QString dateTime = timestamp.toString();

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(entry.id)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(dateTime));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(locale.formattedDataSize(entry.size)));
    }

    ui->tableWidget->sortItems(0);
}

void LogsWindow::onLogDownloadProgressUpdated(const LogEntry& entry, uint32_t bytesLoaded) {
    QDateTime currentTime = QDateTime::currentDateTime();
    double downloadTime = static_cast<double>(_downloadStartTimestamp.msecsTo(currentTime)) / 1000.0;
    QString timeText = QString::number(downloadTime, 'f', 1);
    QString rateText = this->locale().formattedDataSize(static_cast<double>(bytesLoaded) / downloadTime);
    ui->downloadTime->setText(timeText + " s (" + rateText + "/s)");
    if (entry.size > 0) {
        ui->progressBar->setValue((static_cast<float>(bytesLoaded) * 100.0f) / static_cast<float>(entry.size));
        ui->recievedBytes->setText(QString::number(bytesLoaded) + "/" + QString::number(entry.size) + " bytes");
    }
}

void LogsWindow::onLogFileDownloaded(const LogEntry& entry, const QByteArray& data) {
    ui->progressBar->setValue(100);

    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString filePath = downloadsPath + "/log_"+ QString::number(entry.timestamp) +".bin";

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qint64 bytesWritten = file.write(data);
        file.close();
        qDebug() << "Log saved. Successfully wrote" << bytesWritten << "bytes to" << filePath;
    } else {
        qWarning() << "Failed to save log. Error opening file:" << file.errorString();
    }
}

void LogsWindow::onLogSystemStateChanged(VehicleLogs::State newState, VehicleLogs::State oldState) {
    // reset to default state
    ui->buttonRefresh->setDisabled(false);
    ui->buttonDownloadSelected->setDisabled(false);
    ui->buttonDownloadLast->setDisabled(false);
    ui->buttonClearLogs->setDisabled(false);
    ui->buttonCancelDownload->setDisabled(false);

    if (newState == VehicleLogs::State::DownloadingLog) {
        ui->buttonRefresh->setDisabled(true);
        ui->buttonDownloadSelected->setDisabled(true);
        ui->buttonDownloadLast->setDisabled(true);
        ui->buttonClearLogs->setDisabled(true);
    }
    else if (newState == VehicleLogs::State::FetchingLogsList || newState == VehicleLogs::State::ErasingLogs) {
        ui->buttonRefresh->setDisabled(true);
        ui->buttonDownloadSelected->setDisabled(true);
        ui->buttonDownloadLast->setDisabled(true);
        ui->buttonClearLogs->setDisabled(true);
        ui->buttonCancelDownload->setDisabled(true);
    }
}

void LogsWindow::on_buttonRefresh_clicked() {
    ui->tableWidget->setRowCount(0);
    emit requestLogList();
}

void LogsWindow::on_buttonDownloadSelected_clicked() {
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.size() == 0) {
        return;
    }
    uint32_t id = selectedItems[0]->text().toUInt();

    _downloadStartTimestamp = QDateTime::currentDateTime();

    emit requestLogDownload(id);
}

void LogsWindow::on_buttonDownloadLast_clicked() {
    _downloadStartTimestamp = QDateTime::currentDateTime();

    emit requestLogDownload(ui->tableWidget->rowCount());
}

void LogsWindow::on_buttonClearLogs_clicked() {
    QMessageBox::StandardButton pressed = QMessageBox::warning(
        nullptr, "Accept clear", "Would you like to clear all logs from the vehicle?",
        QMessageBox::Ok | QMessageBox::Cancel);

    switch (pressed) {
    case QMessageBox::Ok:
        ui->tableWidget->setRowCount(0);
        emit requestLogErase();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void LogsWindow::on_buttonAnalyzeLog_clicked() {
    LogPlotWindow::openFileToReview();
}

void LogsWindow::on_buttonCancelDownload_clicked() {
    ui->progressBar->setValue(0);
    ui->recievedBytes->setText(QString("Download stopped"));

    emit requestLogDownloadStop();
}
