#include "logswindow.h"
#include "ui_logswindow.h"

#include <QMessageBox>
#include <QStandardPaths>
#include <QFileDialog>

#include <algorithm>

#include <common/mavlink.h>

#include "logplotwindow.h"

std::bitset<LogsWindow::LOGS_MASK_SIZE> LogsWindow::_logsDataMask;

LogsWindow::LogsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogsWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<mavlink_message_t>("mavlink_message_t");

    connect(&_logEntriesTimeout, &QTimer::timeout, this, &LogsWindow::requestMissingLogEntries);
    connect(&_logDataTimeout, &QTimer::timeout, this, &LogsWindow::requestMissingLogPackets);
}

LogsWindow::~LogsWindow() {
    delete ui;
}

void LogsWindow::onAutopilotHeartbeat(const mavlink_message_t& msg) {
    _sysId = msg.sysid;
    _compId = msg.compid;
}

void LogsWindow::handleMavlink(const mavlink_log_entry_t& msg) {
    if (_state != State::FetchingLogsList) {
        qDebug() << "Unexpected log entry recieved";
        return;
    }

    if (msg.id == 0) {
        _logEntriesTimeout.stop();
        changeState(State::Idle);
        qDebug() << "Vihecle has no logs";
        return;
    }

    _logEntriesTimeout.start(_logEntriesTimeoutMillis);

    LogEntry entry;
    entry.id = msg.id;
    entry.timestamp = msg.time_utc;
    entry.size = msg.size;

    if (auto it = std::find_if(_logEntries.begin(), _logEntries.end(), [msg](const LogEntry& entry) {
            return msg.id == entry.id;
        }); it != _logEntries.end()) {
        *it = entry;
    }
    else {
        _logEntries.push_back(entry);
    }

    QLocale locale = this->locale();
    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(QString(msg.id), Qt::MatchExactly);
    if (!items.empty()) {
        QDateTime timestamp;
        timestamp.setSecsSinceEpoch(msg.time_utc);
        QString dateTime = timestamp.toString();

        int32_t row = items[0]->row();
        ui->tableWidget->item(row, 0)->setText(QString::number(msg.id));
        ui->tableWidget->item(row, 1)->setText(dateTime);
        ui->tableWidget->item(row, 2)->setText(locale.formattedDataSize(msg.size));
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

    if (_logEntries.size() > msg.num_logs) {
        _logEntriesTimeout.stop();
        changeState(State::Idle);
        qWarning() << "Log entry reports less logs then already recieved: " << _logEntries.size() << "/" << msg.num_logs;
        return;
    }

    if (_logEntries.size() == msg.num_logs) {
        ui->tableWidget->sortItems(0);
        _logEntriesTimeout.stop();
        changeState(State::Idle);
        qDebug() << "Finished recieving log entries: " << _logEntries.size();
    }
}

void LogsWindow::handleMavlink(const mavlink_log_data_t& logData) {
    if (_state != State::DownloadingLog) {
        qDebug() << "Unexpected log data recieved";
        return;
    }

    if (logData.id == 0 || logData.id > _logEntries.size()) {
        qDebug() << "Received invalid log data id: " << logData.id;
        return;
    }

    if (_receivingLogId != logData.id) {
        qDebug() << "Received log data with wrong id: " << logData.id;
        return;
    }

    _logDataTimeout.start(_logDataTimeoutMillis);

    _logDataBuffer.replace(logData.ofs, logData.count, (const char*)logData.data, logData.count);

    for (int32_t i = logData.ofs; i < logData.ofs + logData.count; i++) {
        _logsDataMask.set(i, true);
    }

    _dataReceivedBytes += logData.count;
    ui->progressBar->setValue((static_cast<float>(_dataReceivedBytes) * 100.0f) / static_cast<float>(_logDataBuffer.size()));
    ui->recievedBytes->setText(QString::number(_dataReceivedBytes) +"/" + QString::number(_logDataBuffer.size()) + " bytes");

    QDateTime currentTime = QDateTime::currentDateTime();
    double downloadTime = static_cast<double>(_downloadStartTimestamp.msecsTo(currentTime)) / 1000.0;
    QString timeText = QString::number(downloadTime, 'f', 1);
    QString rateText = this->locale().formattedDataSize(static_cast<double>(_dataReceivedBytes) / downloadTime);
    ui->downloadTime->setText(timeText + " s (" + rateText + "/s)");

    if (_dataReceivedBytes > _logDataBuffer.size()) {
        qWarning() << "Recieved more log data than expected";
    }

    if (logData.count == 0 || _dataReceivedBytes == _logDataBuffer.size()) {
        _logDataTimeout.stop();

        ui->progressBar->setValue(100);

        QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QString filePath = "";
        if (LogEntry* entry = getLogEntry(_receivingLogId)) {
            filePath = downloadsPath + "/log_"+ QString::number(entry->timestamp) +".bin";
        }
        else {
            qWarning() << "Failed to get log entry with id: " << _receivingLogId;
            filePath = downloadsPath + "/log_.bin";
        }

        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qint64 bytesWritten = file.write(_logDataBuffer);
            file.close();
            qDebug() << "Successfully wrote" << bytesWritten << "bytes to" << filePath;
        } else {
            qWarning() << "Failed to save log. Error opening file:" << file.errorString();
        }

        stopLogTransfer();

        return;
    }
}

void LogsWindow::onActiveDeviceChanged(QStringView deviceName) {
    _logEntriesTimeout.stop();
    _logDataTimeout.stop();
    if (_state == State::DownloadingLog) {
        stopLogTransfer();
    }
    changeState(State::Idle);
    ui->progressBar->setValue(0);
    ui->recievedBytes->setText(QString("Download stopped"));
}

void LogsWindow::refreshLogs() {
    if (_state != State::Idle) {
        qWarning() << "Trying to refresh log while busy";
        return;
    }

    qDebug() << "Logs list requested";

    changeState(State::FetchingLogsList);

    ui->tableWidget->setRowCount(0);
    _logEntries.clear();

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

    emit sendCommand(command);

    _logEntriesTimeout.start(_logEntriesTimeoutMillis);
}

void LogsWindow::downloadLog(uint32_t id) {
    if (_state != State::Idle) {
        qWarning() << "Can not download log, while not in idle state";
        return;
    }

    if (id == 0 || id > _logEntries.size()) {
        qWarning() << "Requested log download for invalid id: " << id;
        return;
    }

    LogEntry* entry = getLogEntry(id);
    if (!entry) {
        qWarning() << "Failed to find log entry with specified id: " << id;
        return;
    }

    qDebug() << "Requested log download: " << id;

    changeState(State::DownloadingLog);

    ui->buttonRefresh->setDisabled(true);
    ui->buttonDownloadSelected->setDisabled(true);
    ui->buttonDownloadLast->setDisabled(true);
    ui->buttonClearLogs->setDisabled(true);
    ui->buttonAnalyzeLog->setDisabled(true);

    _receivingLogId = id;
    _dataReceivedBytes = 0;
    _logDataBuffer.resize(entry->size);

    mavlink_message_t command;
    mavlink_msg_log_request_data_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        _sysId,
        _compId,
        id,
        0,
        entry->size
        );

    emit sendCommand(command);

    _logDataTimeout.start(_logDataTimeoutMillis);

    _downloadStartTimestamp = QDateTime::currentDateTime();
}

void LogsWindow::clearLogs() {
    if (_state != State::Idle) {
        qWarning() << "Can not cleat logs, while not in idle state";
        return;
    }

    ui->tableWidget->setRowCount(0);
    _logEntries.clear();

    mavlink_message_t command;
    mavlink_msg_log_erase_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        _sysId,
        _compId
    );

    emit sendCommand(command);
}

void LogsWindow::stopLogTransfer() {
    if (_state != State::DownloadingLog) {
        qWarning() << "Not in log download state";
        return;
    }

    _logDataTimeout.stop();
    _receivingLogId = 0;
    _logDataBuffer.resize(0);
    _logsDataMask.reset();

    mavlink_message_t command;
    mavlink_msg_log_request_end_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        _sysId,
        _compId
    );

    emit sendCommand(command);

    ui->buttonRefresh->setDisabled(false);
    ui->buttonDownloadSelected->setDisabled(false);
    ui->buttonDownloadLast->setDisabled(false);
    ui->buttonClearLogs->setDisabled(false);
    ui->buttonAnalyzeLog->setDisabled(false);

    changeState(State::Idle);
}

void LogsWindow::requestMissingLogPackets() {
    if (_state != State::DownloadingLog) {
        qWarning() << "Trying to repeat log download request while not in log download state";
        return;
    }

    qDebug() << "Log packets are missing.";

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
            _receivingLogId,
            segmentStart,
            segmentEnd - segmentStart
        );

        emit sendCommand(command);
    }
}

void LogsWindow::requestMissingLogEntries() {
    if (_state != State::FetchingLogsList) {
        qWarning() << "Trying to repeat log entries fetch while not in log fetching state";
        return;
    }

    qDebug() << "Log entries are missing";

    for (uint32_t i = 1; i < _expectedLogEntries; i++) {
        if (!getLogEntry(i)) {
            qDebug() << "Missing log entry with id: " << i;
        }
    }

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

    emit sendCommand(command);
}

void LogsWindow::on_buttonRefresh_clicked() {
    refreshLogs();
}

void LogsWindow::on_buttonDownloadSelected_clicked() {
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.size() == 0) {
        return;
    }
    uint32_t id = selectedItems[0]->text().toUInt();
    downloadLog(id);
}

void LogsWindow::on_buttonDownloadLast_clicked() {
    downloadLog(_logEntries.size());
}

void LogsWindow::on_buttonClearLogs_clicked() {
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

void LogsWindow::on_buttonAnalyzeLog_clicked() {
    LogPlotWindow::openFileToReview();
}

void LogsWindow::on_buttonCancelDownload_clicked() {
    stopLogTransfer();
    ui->progressBar->setValue(0);
    ui->recievedBytes->setText(QString("Download stopped"));
}

bool LogsWindow::changeState(State state) {
    if (state == _state) {
        qWarning() << "LogsWindow changed state to the same: " << (uint32_t)_state;
        return true;
    }
    else if (_state == State::Idle || state == State::Idle) {
        _state = state;
        return true;
    }
    else {
        qWarning() << "LogsWindow is not allowed to change state from: " << (uint32_t)_state << " to " << (uint32_t)state;
        return false;
    }
}

LogEntry* LogsWindow::getLogEntry(uint32_t id) {
    if (auto it = std::find_if(_logEntries.begin(), _logEntries.end(), [id](const LogEntry& entry) {
            return id == entry.id;
        }); it != _logEntries.end()) {
        return &(*it);
    }
    else {
        return nullptr;
    }
}

