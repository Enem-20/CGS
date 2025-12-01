#ifndef GLOBALPOSITIONINT_H
#define GLOBALPOSITIONINT_H

#include <QObject>
#include <QVector3D>

struct __mavlink_global_position_int_t;
typedef __mavlink_global_position_int_t mavlink_global_position_int_t;

class GlobalPositionInt : public QObject
{
    Q_OBJECT
private:
    int32_t altitude;
public:
    explicit GlobalPositionInt(QObject *parent = nullptr);

    void handleMavlink(const mavlink_global_position_int_t& msg);
signals:
    void altitudeUpdated(int32_t altitude);

};

#endif // GLOBALPOSITIONINT_H
