#ifndef MAVLINKVEHICLELOGS_H
#define MAVLINKVEHICLELOGS_H

#include "vehicle/logs/vehiclelogs.h"

#include "protocols/mavlink/MavlinkFwd.h"
#include "memoryManagement/segmentmap.h"

class MavlinkVehicleLogs : public VehicleLogs {
    Q_OBJECT

public:
    explicit MavlinkVehicleLogs(QObject *parent = nullptr);

protected:
    uint32_t _receivingLogId = 0;
    QByteArray _logDataBuffer;
    SegmentMap _segmentMap;
    uint32_t _dataReceivedBytes = 0;
    QTimer _logDataTimeout;
    static const uint32_t _logDataTimeoutMillis = 2000;

    uint32_t _expectedLogEntries = 0;
    QTimer _logEntriesTimeout;
    static const uint32_t _logEntriesTimeoutMillis = 2000;

    QTimer _logErasingTimeout;
    static const uint32_t _logErasingTimeoutMillis = 3000;

public slots:
    void onMessage(Message msg) override;
    void onLogEntryReceived(Message msg) override;
    void onLogDataReceived(Message msg) override;
    void onActiveDeviceChanged(QStringView deviceName) override;
    void onLogsListRequested() override;
    void onLogDownloadRequested(uint32_t id) override;
    void onLogDownloadStopRequested() override;
    void onEraseLogsRequested() override;

    void handleMavlink(const mavlink_log_entry_t& msg);
    void handleMavlink(const mavlink_log_data_t& msg);
    
private slots:
    void stopLogDownload();
    void requestMissingLogEntries();
    void requestMissingLogPackets();
    void erasingLogsCheck();
};

#endif // MAVLINKVEHICLELOGS_H
