#include "logswindow.h"
#include "ui_logswindow.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QStandardPaths>
#include <QFileDialog>

#include <common/mavlink.h>

std::bitset<LogsWindow::LOGS_MASK_SIZE> LogsWindow::_logsDataMask;

LogsWindow::LogsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogsWindow)
{
    ui->setupUi(this);

    connect(&_logsTimeout, &QTimer::timeout, this, &LogsWindow::requestMissingLogPackets);
}

LogsWindow::~LogsWindow() {
    delete ui;
}

void LogsWindow::setMavlinkContext(MavlinkContext* mavlinkContext) {
    _mavlinkContext = mavlinkContext;
}

void LogsWindow::on_pushButton_clicked() {
    refreshLogs();
}

void LogsWindow::on_pushButton_2_clicked() {
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.size() == 0) return;
    int32_t id = selectedItems[0]->row() + 1;
    downloadLog(id);
}

void LogsWindow::on_pushButton_3_clicked() {
    downloadLog(_logEntries.size());
}

void LogsWindow::on_pushButton_4_clicked() {
    QMessageBox::StandardButton pressed = QMessageBox::warning(
        nullptr, "Accept clear", "Would you like to clear all logs from the vehicle?",
        QMessageBox::Ok | QMessageBox::Cancel);

    switch (pressed) {
    case QMessageBox::Ok:
        clearLogs();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void LogsWindow::on_pushButton_5_clicked() {
    stopLogTransfer();
    ui->progressBar->setValue(0);
    ui->recievedBytes->setText(QString("Download stopped"));
}

void LogsWindow::on_pushButton_6_clicked() {
    if (_logPlotWindow.isHidden()) {
        _logPlotWindow.wrapShow();
    }
}

void LogsWindow::onAutopilotHeartbeat(const mavlink_message_t& msg) {
    _sysId = msg.sysid;
    _compId = msg.compid;
}

void LogsWindow::handleMavlink(const mavlink_log_entry_t& msg) {
    if (msg.id == 0) return;

    LogEntry entry;
    entry.id = msg.id;
    entry.timestamp = msg.time_utc;
    entry.size = msg.size;
    _logEntries.push_back(entry);

    QLocale locale = this->locale();
    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(QString(msg.id), Qt::MatchExactly);
    if (!items.empty()) {
        QDateTime timestamp;
        timestamp.setSecsSinceEpoch(msg.time_utc);
        QString dateTime = timestamp.toString();

        QTableWidgetItem* parameterItemName = items[0];
        ui->tableWidget->setItem(parameterItemName->row(), 0, new QTableWidgetItem(QString::number(msg.id)));
        ui->tableWidget->setItem(parameterItemName->row(), 1, new QTableWidgetItem(dateTime));
        ui->tableWidget->setItem(parameterItemName->row(), 2, new QTableWidgetItem(locale.formattedDataSize(msg.size)));
    }
    else {
        QDateTime timestamp;
        timestamp.setSecsSinceEpoch(msg.time_utc);
        QString dateTime = timestamp.toString();

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(msg.id)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(dateTime));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(locale.formattedDataSize(msg.size)));
    }
}

void LogsWindow::handleMavlink(const mavlink_log_data_t& logData, const mavlink_message_t& msg) {
    if (_receivingDataId == 0xffffffff || _receivingDataId != logData.id) return;

    _logDataBuffer.replace(logData.ofs, logData.count, (const char*)logData.data, logData.count);

    for (int32_t i = logData.ofs; i < logData.ofs + logData.count; i++) {
        _logsDataMask.set(i, true);
    }

    _dataReceivedBytes += logData.count;
    ui->progressBar->setValue(((float)_dataReceivedBytes * 100.0f) / (float)_logDataBuffer.size());
    ui->recievedBytes->setText(QString::number(_dataReceivedBytes) +"/" + QString::number(_logDataBuffer.size()) + " bytes");

    _logsTimeout.start(5000);

    if (_dataReceivedBytes > _logDataBuffer.size()) {
        throw std::exception();
    }

    if (logData.count == 0 || _dataReceivedBytes == _logDataBuffer.size()) {
        _logsTimeout.stop();

        ui->progressBar->setValue(100);

        QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QString filePath = downloadsPath + "/log_"+ QString::number(_logEntries[_receivingDataId - 1].timestamp) +".bin";

        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qint64 bytesWritten = file.write(_logDataBuffer);
            file.close();
            qDebug() << "Successfully wrote" << bytesWritten << "bytes to" << filePath;
        } else {
            qDebug() << "Error opening file:" << file.errorString();
        }

        stopLogTransfer();

        return;
    }
}

void LogsWindow::refreshLogs() {
    ui->tableWidget->setRowCount(0);
    _logEntries.clear();

    if (!_mavlinkContext) return;

    mavlink_message_t command;
    mavlink_msg_log_request_list_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        _sysId,
        _compId,
        0,
        0xffff
    );

    _mavlinkContext->sendCommand(command);
}

void LogsWindow::downloadLog(uint32_t id) {
    if (!_mavlinkContext) return;

    if (_logDataBuffer.size() > 0) return;

    if (id < 1 || id > _logEntries.size()) return;

    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
    ui->pushButton_6->setDisabled(true);

    //_logsDataMask = new std::bitset<LOGS_MASK_SIZE>;

    _receivingDataId = id;
    _dataReceivedBytes = 0;
    _logDataBuffer.resize(_logEntries[id - 1].size);

    mavlink_message_t command;
    mavlink_msg_log_request_data_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        _sysId,
        _compId,
        id,
        0,
        _logEntries[id - 1].size
        );

    _mavlinkContext->sendCommand(command);

    _logsTimeout.start(2000);
}

void LogsWindow::clearLogs() {
    ui->tableWidget->setRowCount(0);
    _logEntries.clear();

    if (!_mavlinkContext) return;

    mavlink_message_t command;
    mavlink_msg_log_erase_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        _sysId,
        _compId
    );

    _mavlinkContext->sendCommand(command);
}

void LogsWindow::stopLogTransfer() {    
    _logsTimeout.stop();
    _receivingDataId = 0xffffffff;
    _logDataBuffer.resize(0);
    _logsDataMask.reset();

    if (!_mavlinkContext) return;

    mavlink_message_t command;
    mavlink_msg_log_request_end_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        _sysId,
        _compId
    );

    _mavlinkContext->sendCommand(command);

    ui->pushButton->setDisabled(false);
    ui->pushButton_2->setDisabled(false);
    ui->pushButton_3->setDisabled(false);
    ui->pushButton_4->setDisabled(false);
    ui->pushButton_6->setDisabled(false);
}

void LogsWindow::requestMissingLogPackets() {
    qDebug() << "Not all packets arrived.";
    for (size_t i = 0; i < _logDataBuffer.size();) {
        if (_logsDataMask.test(i)) {
            i++;
            continue;
        }
        size_t segmentStart = i;
        while(!_logsDataMask.test(++i) && i < _logDataBuffer.size()) {}
        size_t segmentEnd = i;

        qDebug() << "Missing segment: " << segmentStart << " " << segmentEnd;

        mavlink_message_t command;
        mavlink_msg_log_request_data_pack(
            255,
            MAV_COMP_ID_MISSIONPLANNER,
            &command,
            _sysId,
            _compId,
            _receivingDataId,
            segmentStart,
            segmentEnd - segmentStart
        );

        _mavlinkContext->sendCommand(command);
    }
}
