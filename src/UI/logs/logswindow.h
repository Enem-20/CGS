#ifndef LOGSWINDOW_H
#define LOGSWINDOW_H

#include <QWidget>

#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QTimer>
#include <QDateTime>

#include <bitset>

namespace Ui {
class LogsWindow;
}

struct __mavlink_heartbeat_t;
typedef __mavlink_heartbeat_t mavlink_heartbeat_t;

struct __mavlink_message;
typedef __mavlink_message mavlink_message_t;

struct __mavlink_log_request_list_t;
typedef __mavlink_log_request_list_t mavlink_log_request_list_t;

struct __mavlink_log_entry_t;
typedef __mavlink_log_entry_t mavlink_log_entry_t;

struct __mavlink_log_request_data_t;
typedef __mavlink_log_request_data_t mavlink_log_request_data_t;

struct __mavlink_log_data_t;
typedef __mavlink_log_data_t mavlink_log_data_t;

struct __mavlink_log_erase_t;
typedef __mavlink_log_erase_t mavlink_log_erase_t;

struct __mavlink_log_request_end_t;
typedef __mavlink_log_request_end_t mavlink_log_request_end_t;

class MavlinkContext;

struct LogEntry {
    uint16_t id;
    uint32_t timestamp;
    uint32_t size;
};

class LogsWindow : public QWidget {
    Q_OBJECT

    enum class State {
        Idle = 0,
        FetchingLogsList,
        DownloadingLog
    };

public:
    explicit LogsWindow(QWidget *parent = nullptr);
    ~LogsWindow();

signals:
    void sendCommand(const mavlink_message_t& msg);

private slots:
    void on_buttonRefresh_clicked();
    void on_buttonDownloadSelected_clicked();
    void on_buttonDownloadLast_clicked();
    void on_buttonClearLogs_clicked();
    void on_buttonAnalyzeLog_clicked();
    void on_buttonCancelDownload_clicked();

public slots:
    void onAutopilotHeartbeat(const mavlink_message_t& msg);
    void handleMavlink(const mavlink_log_entry_t& msg);
    void handleMavlink(const mavlink_log_data_t& logData);
    void onActiveDeviceChanged(QStringView deviceName);

private:
    Ui::LogsWindow *ui;
    uint8_t _sysId = 0;
    uint8_t _compId = 0;
    State _state = State::Idle;

    static constexpr size_t LOGS_MASK_SIZE = 1024 * 1024 * 1024;
    static std::bitset<LOGS_MASK_SIZE> _logsDataMask;

    QVector<LogEntry> _logEntries;
    uint32_t _receivingLogId = 0;
    QByteArray _logDataBuffer;
    uint32_t _dataReceivedBytes = 0;
    QDateTime _downloadStartTimestamp;
    QTimer _logDataTimeout;
    static const uint32_t _logDataTimeoutMillis = 2000;

    uint32_t _expectedLogEntries = 0;
    QTimer _logEntriesTimeout;
    static const uint32_t _logEntriesTimeoutMillis = 2000;

    void refreshLogs();
    void downloadLog(uint32_t id);
    void clearLogs();
    void stopLogTransfer();

    void requestMissingLogPackets();
    void requestMissingLogEntries();

    bool changeState(State state);
    LogEntry* getLogEntry(uint32_t id);
};

#endif // LOGSWINDOW_H
