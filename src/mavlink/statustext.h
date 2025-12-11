#ifndef STATUSTEXT_H
#define STATUSTEXT_H

#include <QObject>
#include <QString>

struct __mavlink_statustext_t;
typedef __mavlink_statustext_t mavlink_statustext_t;

class StatusText : public QObject
{
    Q_OBJECT
public:
    explicit StatusText(QObject *parent = nullptr);
    void handleMavlink(const mavlink_statustext_t& msg);
signals:
    void logUpdated(QString msg, QString severity);
public slots:

};

#endif // STATUSTEXT_H
