#ifndef LOGSWINDOW_H
#define LOGSWINDOW_H

#include <QFile>
#include <QDataStream>
#include <QByteArray>

#include <QWidget>

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

class LogsWindow : public QWidget
{
    Q_OBJECT
private:
    uint8_t _sysId = 0;
    uint8_t _compId = 0;
    MavlinkContext* _mavlinkContext = nullptr;

public:
    explicit LogsWindow(QWidget *parent = nullptr);
    ~LogsWindow();

    void setMavlinkContext(MavlinkContext* mavlinkContext);

signals:
    void refreshLogs(const mavlink_message_t& msg);
    void requestLogData(const mavlink_message_t& msg);
    void requestLogEnd(const mavlink_message_t& msg);
    void eraseAllLogs(const mavlink_message_t& msg);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

public slots:
    void onAutopilotHeartbeat(const mavlink_message_t& msg);
    void handleMavlink(const mavlink_log_entry_t& msg);
    void handleMavlink(const mavlink_log_data_t& logData, const mavlink_message_t& msg);

private:
    Ui::LogsWindow *ui;
    QList<LogEntry> _logEntries;
    uint32_t _receivingDataId = 0xffffffff;
    uint32_t _dataReceivedBytes = 0;
    QByteArray _logDataBuffer;
    QList<uint8_t> _packetSeq;

    void refreshLogs();
    void downloadLog(uint32_t id);
    void clearLogs();
    void stopLogTransfer();
};

#endif // LOGSWINDOW_H
