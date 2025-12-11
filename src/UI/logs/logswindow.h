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
private:
    uint8_t _sysId = 0;
    uint8_t _compId = 0;
    QDateTime _downloadStartTimestamp;

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
    QList<LogEntry> _logEntries;
    uint32_t _receivingDataId = 0xffffffff;
    uint32_t _dataReceivedBytes = 0;
    QByteArray _logDataBuffer;
    static constexpr size_t LOGS_MASK_SIZE = 1024 * 1024 * 1024;
    static std::bitset<LOGS_MASK_SIZE> _logsDataMask;
    QTimer _logsTimeout;
    static const uint32_t _logsTimeoutMillis = 2000;
    QTimer _logEntriesTimeout;
    static const uint32_t _logEntriesTimeoutMillis = 2000;
    bool _downloadingLog = false;

    void refreshLogs();
    void downloadLog(uint32_t id);
    void clearLogs();
    void stopLogTransfer();
    void requestMissingLogPackets();
};

#endif // LOGSWINDOW_H
