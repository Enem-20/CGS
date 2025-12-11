#ifndef ATTITUDE_H
#define ATTITUDE_H

#include <QObject>
#include <QVector3D>

struct __mavlink_attitude_t;
typedef __mavlink_attitude_t mavlink_attitude_t;

class Attitude : public QObject
{
    Q_OBJECT
private:
    QVector3D _orientation;
public:
    explicit Attitude(QObject *parent = nullptr);
    void handleMavlink(const mavlink_attitude_t& msg);
signals:
    void orientationUpdated(const QVector3D& orientation);
};

#endif // ATTITUDE_H
