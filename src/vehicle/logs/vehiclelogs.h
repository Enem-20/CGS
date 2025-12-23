#ifndef VEHICLELOGS_H
#define VEHICLELOGS_H

#include "protocols/ProtocolSubscriber.h"

#include <QVector>
#include <QTimer>
#include <QDateTime>

struct LogEntry {
    uint64_t timestamp;
    uint32_t id;
    uint32_t size;
};

class VehicleLogs : public ProtocolSubscriber {
    Q_OBJECT

public:
    enum class State {
        Idle = 0,
        FetchingLogsList,
        DownloadingLog,
        ErasingLogs
    };

protected:
    State _state;
    QVector<LogEntry> _logEntries;

    explicit VehicleLogs(QObject *parent = nullptr);

    bool changeState(State state);
    LogEntry* getLogEntry(uint32_t id);

signals:
    void stateChanged(State newState, State oldState);
    void logsListReceived(const QVector<LogEntry>& entries);
    void logFileDownloaded(const LogEntry& entry, const QByteArray& data);
    void logDownloadProgressUpdated(const LogEntry& entry, uint32_t bytesLoaded);
    void logsErased();

public slots:
    virtual void onLogEntryReceived(Message msg) = 0;
    virtual void onLogDataReceived(Message msg) = 0;
    virtual void onActiveDeviceChanged(QStringView deviceName) = 0;
    virtual void onLogsListRequested() = 0;
    virtual void onLogDownloadRequested(uint32_t id) = 0;
    virtual void onLogDownloadStopRequested() = 0;
    virtual void onEraseLogsRequested() = 0;
};

#endif // VEHICLELOGS_H
