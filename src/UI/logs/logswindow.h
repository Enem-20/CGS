#ifndef LOGSWINDOW_H
#define LOGSWINDOW_H

#include <QWidget>

#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QTimer>
#include <QDateTime>

#include "vehicle/logs/vehiclelogs.h"

namespace Ui {
class LogsWindow;
}

class LogsWindow : public QWidget {
    Q_OBJECT

public:
    explicit LogsWindow(QWidget *parent = nullptr);
    ~LogsWindow();

private:
    QDateTime _downloadStartTimestamp;

signals:
    void requestLogList();
    void requestLogDownload(uint32_t id);
    void requestLogErase();
    void requestLogDownloadStop();

public slots:
    void onActiveDeviceChanged(QStringView deviceName);
    void onLogListReceived(const QVector<LogEntry>& entries);
    void onLogDownloadProgressUpdated(const LogEntry& entry, uint32_t bytesLoaded);
    void onLogFileDownloaded(const LogEntry& entry, const QByteArray& data);
    void onLogSystemStateChanged(VehicleLogs::State newState, VehicleLogs::State oldState);

private slots:
    void on_buttonRefresh_clicked();
    void on_buttonDownloadSelected_clicked();
    void on_buttonDownloadLast_clicked();
    void on_buttonClearLogs_clicked();
    void on_buttonAnalyzeLog_clicked();
    void on_buttonCancelDownload_clicked();

private:
    Ui::LogsWindow *ui;
};

#endif // LOGSWINDOW_H
