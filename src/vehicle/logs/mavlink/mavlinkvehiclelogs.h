#ifndef MAVLINKVEHICLELOGS_H
#define MAVLINKVEHICLELOGS_H

#include "vehicle/logs/vehiclelogs.h"

#include <bitset>

#include "mavlink/mavlinktypes.h"

class MavlinkVehicleLogs : public VehicleLogs {
    Q_OBJECT

public:
    explicit MavlinkVehicleLogs(QObject *parent = nullptr);

protected:
    static constexpr size_t LOGS_MASK_SIZE = 1024 * 1024 * 1024;
    static std::bitset<LOGS_MASK_SIZE> _logsDataMask;

    uint32_t _receivingLogId = 0;
    QByteArray _logDataBuffer;
    uint32_t _dataReceivedBytes = 0;
    QTimer _logDataTimeout;
    static const uint32_t _logDataTimeoutMillis = 2000;

    uint32_t _expectedLogEntries = 0;
    QTimer _logEntriesTimeout;
    static const uint32_t _logEntriesTimeoutMillis = 2000;

public slots:
    void onMessageReceived(Message msg) override;
    void onActiveDeviceChanged(QStringView deviceName) override;
    void onLogsListRequested() override;
    void onLogDownloadRequested(uint32_t id) override;
    void onLogDownloadStopRequested() override;
    void onEraseLogsRequested() override;

private slots:
    void handleMavlink(const mavlink_log_entry_t& msg);
    void handleMavlink(const mavlink_log_data_t& msg);
    void stopLogDownload();
    void requestMissingLogEntries();
    void requestMissingLogPackets();
};

#endif // MAVLINKVEHICLELOGS_H
