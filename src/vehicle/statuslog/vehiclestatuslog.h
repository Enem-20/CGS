#ifndef VEHICLESTATUSLOG_H
#define VEHICLESTATUSLOG_H

#include "protocols/ProtocolSubscriber.h"

#include "protocols/message.h"

class VehicleStatusLog : public ProtocolSubscriber {
    Q_OBJECT

public:
    explicit VehicleStatusLog(QObject *parent = nullptr);

signals:
    void statusMessageReceived(const QString& text, const QString& severity);
};

#endif // VEHICLESTATUSLOG_H
