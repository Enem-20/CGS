#include "mavlinkvehiclelogs.h"

#include <QDebug>

#include <common/mavlink.h>

std::bitset<MavlinkVehicleLogs::LOGS_MASK_SIZE> MavlinkVehicleLogs::_logsDataMask;

MavlinkVehicleLogs::MavlinkVehicleLogs(QObject *parent)
    : VehicleLogs{parent}
{
    connect(&_logEntriesTimeout, &QTimer::timeout, this, &MavlinkVehicleLogs::requestMissingLogEntries);
    connect(&_logDataTimeout, &QTimer::timeout, this, &MavlinkVehicleLogs::requestMissingLogPackets);
}

void MavlinkVehicleLogs::onMessageReceived(Message msg) {
    MavlinkMessage* message = msg.read<MavlinkMessage>();
    switch (message->msgid) {
    case MAVLINK_MSG_ID_LOG_ENTRY: {
        mavlink_log_entry_t logEntry;
        mavlink_msg_log_entry_decode(message, &logEntry);
        handleMavlink(logEntry);
        break;
    }
    case MAVLINK_MSG_ID_LOG_DATA: {
        mavlink_log_data_t logData;
        mavlink_msg_log_data_decode(message, &logData);
        handleMavlink(logData);
        break;
    }
    }
}

void MavlinkVehicleLogs::onActiveDeviceChanged(QStringView deviceName) {
    _logEntriesTimeout.stop();
    _logDataTimeout.stop();
    if (_state == State::DownloadingLog) {
        stopLogDownload();
    }
    changeState(State::Idle);
}

void MavlinkVehicleLogs::onLogsListRequested() {
    if (_state != State::Idle) {
        qWarning() << "Trying to refresh log while busy";
        return;
    }

    qDebug() << "Logs list requested";

    changeState(State::FetchingLogsList);

    _logEntries.clear();

    mavlink_message_t command;
    mavlink_msg_log_request_list_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        0,
        0,
        0,
        0xffff
    );

    Message message;
    message.write(&command);
    emit sendMessage(message);

    _logEntriesTimeout.start(_logEntriesTimeoutMillis);
}

void MavlinkVehicleLogs::onLogDownloadRequested(uint32_t id) {
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

    _receivingLogId = id;
    _dataReceivedBytes = 0;
    _logDataBuffer.resize(entry->size);

    mavlink_message_t command;
    mavlink_msg_log_request_data_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        0,
        0,
        id,
        0,
        entry->size
    );

    Message message;
    message.write(&command);
    emit sendMessage(message);

    _logDataTimeout.start(_logDataTimeoutMillis);
}

void MavlinkVehicleLogs::onLogDownloadStopRequested() {
    stopLogDownload();
}

void MavlinkVehicleLogs::onEraseLogsRequested() {
    if (_state != State::Idle) {
        qWarning() << "Can not cleat logs, while not in idle state";
        return;
    }

    _logEntries.clear();

    mavlink_message_t command;
    mavlink_msg_log_erase_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        0,
        0
    );

    Message message;
    message.write(&command);
    emit sendMessage(message);}

void MavlinkVehicleLogs::handleMavlink(const mavlink_log_entry_t& msg) {
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

    if (_logEntries.size() > msg.num_logs) {
        _logEntriesTimeout.stop();
        changeState(State::Idle);
        qWarning() << "Log entry reports less logs then already recieved: " << _logEntries.size() << "/" << msg.num_logs;
        return;
    }

    if (_logEntries.size() == msg.num_logs) {
        _logEntriesTimeout.stop();
        changeState(State::Idle);
        qDebug() << "Finished recieving log entries: " << _logEntries.size();
    }
}

void MavlinkVehicleLogs::handleMavlink(const mavlink_log_data_t& msg) {
    if (_state != State::DownloadingLog) {
        qDebug() << "Unexpected log data recieved";
        return;
    }

    if (msg.id == 0 || msg.id > _logEntries.size()) {
        qDebug() << "Received invalid log data id: " << msg.id;
        return;
    }

    if (_receivingLogId != msg.id) {
        qDebug() << "Received log data with wrong id: " << msg.id;
        return;
    }

    _logDataTimeout.start(_logDataTimeoutMillis);

    _logDataBuffer.replace(msg.ofs, msg.count, (const char*)msg.data, msg.count);

    for (int32_t i = msg.ofs; i < msg.ofs + msg.count; i++) {
        _logsDataMask.set(i, true);
    }

    _dataReceivedBytes += msg.count;

    emit logDownloadProgressUpdated(*getLogEntry(_dataReceivedBytes), _dataReceivedBytes);

    if (_dataReceivedBytes > _logDataBuffer.size()) {
        qWarning() << "Received more log data than expected: " << _dataReceivedBytes << "/" <<  _logDataBuffer.size();
    }

    if (msg.count == 0 || _dataReceivedBytes == _logDataBuffer.size()) {
        emit logFileDownloaded(*getLogEntry(_dataReceivedBytes), _logDataBuffer);
        stopLogDownload();
        return;
    }
}

void MavlinkVehicleLogs::stopLogDownload() {
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
        0,
        0
    );

    Message message;
    message.write(&command);
    emit sendMessage(message);

    changeState(State::Idle);
}

void MavlinkVehicleLogs::requestMissingLogEntries() {
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
        0,
        0,
        0,
        0xffff
    );

    Message message;
    message.write(&command);
    emit sendMessage(message);}


void MavlinkVehicleLogs::requestMissingLogPackets() {
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
            0,
            0,
            _receivingLogId,
            segmentStart,
            segmentEnd - segmentStart
        );

        Message message;
        message.write(&command);
        emit sendMessage(message);
    }
}
