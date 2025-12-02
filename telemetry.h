#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <QObject>

struct __mavlink_message;
typedef __mavlink_message mavlink_message_t;

class Telemetry : public QObject
{
    Q_OBJECT
private:
    QStringList _telemetryQueue;
public:
    explicit Telemetry(QObject *parent = nullptr);
signals:
    void updated(const QStringList& telemetryQueue);
public slots:

};

#endif // TELEMETRY_H
