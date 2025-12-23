#ifndef C_PARAMETERS_H
#define C_PARAMETERS_H

#include <QHash>
#include <QTimer>

#include "protocols/message.h"
#include "protocols/ProtocolSubscriber.h"
#include "memoryManagement/segmentmap.h"

class Parameter;

class Parameters : public ProtocolSubscriber {
    Q_OBJECT
    QHash<uint16_t, Parameter*> _parameters;
    QHash<QString, Parameter*> _parametersById;
    SegmentMap _segmentMap;
    QTimer _timerPullTimeout;
    QTimer _pushTimeoutTimer;
    QTimer _syncTimer;
    bool _pulledOnConnect = false;
public:
    Parameters(QObject* parent = nullptr) noexcept;
    ~Parameters();

signals:
    void newParameterReceived(Parameter* parameter);
    void parameterUpdated(Parameter* parameter);
    void parametersPullingStarted();
    void parametersPullingCompleted();

public slots:
    void saveToFile(const QString& path);
    void loadFromFile();

    void downloadLost();
    void uploadLost();

    void onMessage(Message msg) override;
    void onErase();

    void clear();

    void onSendSingleToVehicle(uint16_t index);
    void onSendToVehicle();
    
    void onRequestSingleFromVehicle(uint16_t index);
    void onRequestAllFromVehicle();
    void onConnect();
    void onDisconnect();
};

#endif // C_PARAMETERS_HPP