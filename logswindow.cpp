#include "logswindow.h"
#include "ui_logswindow.h"
#include "mainwindow.h"

#include <QMessageBox>

#include <common/mavlink.h>

LogsWindow::LogsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogsWindow)
{
    ui->setupUi(this);
}

LogsWindow::~LogsWindow() {
    delete ui;
}

void LogsWindow::setMavlinkContext(MavlinkContext* mavlinkContext) {
    _mavlinkContext = mavlinkContext;
}

// void LogsWindow::refreshLogs(const mavlink_message_t& msg) {

// }

// void LogsWindow::requestLogData(const mavlink_message_t& msg) {

// }

// void LogsWindow::requestLogEnd(const mavlink_message_t& msg) {

// }

// void LogsWindow::eraseAllLogs(const mavlink_message_t& msg) {

// }

void LogsWindow::on_pushButton_clicked() {
    refreshLogs();
}

void LogsWindow::on_pushButton_2_clicked() {
    if (_packetSeq.size() > 0) {
        uint8_t cur = _packetSeq[0];
        for (int32_t i = 1; i < _packetSeq.size(); i++) {
            if(uint8_t(cur + 1) != (uint8_t)_packetSeq[i]) {
                qDebug() << "Missing packet: " << i <<"\n";
            }
            cur = _packetSeq[i];
        }
    }

    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.size() == 0) return;
    int32_t id = selectedItems[0]->row() + 1;
    downloadLog(id);
}

void LogsWindow::on_pushButton_3_clicked() {
    downloadLog(_logEntries.size());
}

void LogsWindow::on_pushButton_4_clicked() {
    QMessageBox::StandardButton pressed = QMessageBox::warning(nullptr, "Accept clear", "Would you like to clear all logs from the vehicle?",
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

void LogsWindow::onAutopilotHeartbeat(const mavlink_message_t& msg) {
    _sysId = msg.sysid;
    _compId = msg.compid;
}

void LogsWindow::handleMavlink(const mavlink_log_entry_t& msg) {
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

    _packetSeq.push_back(msg.seq);

    _logDataBuffer.replace(logData.ofs, logData.count, (const char*)logData.data, logData.count);

    _dataReceivedBytes += logData.count;
    ui->progressBar->setValue(((float)_dataReceivedBytes * 100.0f) / (float)_logDataBuffer.size());

    if (_dataReceivedBytes > _logDataBuffer.size()) {
        throw std::exception();
    }

    if (logData.count == 0 || _dataReceivedBytes == _logDataBuffer.size()) {
        ui->progressBar->setValue(100);

        QString filePath = "log_"+ QString::number(_logEntries[_receivingDataId - 1].timestamp) +".bin";

        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qint64 bytesWritten = file.write(_logDataBuffer);
            file.close();
            qDebug() << "Successfully wrote" << bytesWritten << "bytes to" << filePath;
        } else {
            qDebug() << "Error opening file:" << file.errorString();
        }

        _receivingDataId = 0xffffffff;
        _logDataBuffer.resize(0);

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

    if(_logDataBuffer.size() > 0) return;

    if(id < 1 || id > _logEntries.size()) return;

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
}
