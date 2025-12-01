#ifndef LOCALPOSITIONNED_H
#define LOCALPOSITIONNED_H

#include <QObject>
#include <QVector3D>

struct __mavlink_local_position_ned_t;
typedef __mavlink_local_position_ned_t mavlink_local_position_ned_t;

class LocalPositionNED : public QObject
{
    Q_OBJECT
private:
    QVector3D _speeds;
public:
    explicit LocalPositionNED(QObject *parent = nullptr);
    void handleMavlink(const mavlink_local_position_ned_t& msg);
signals:
    void speedsUpdated(const QVector3D& orientation);
};

#endif // LOCALPOSITIONNED_H
