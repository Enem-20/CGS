#include "Parameters.h"

#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <common/mavlink.h>

#include "Parameter.h"

Parameters::Parameters(QObject* parent) noexcept
    : ProtocolSubscriber(parent) {
    _pushTimeoutTimer.setInterval(5000);
    _timerPullTimeout.setInterval(5000);
    _syncTimer.setInterval(5000);

    connect(&_syncTimer, &QTimer::timeout, this, [this](){
        if(!_pulledOnConnect) {
            onRequestAllFromVehicle();
            _pulledOnConnect = true;
        }
    });
    connect(&_timerPullTimeout, &QTimer::timeout, this, &Parameters::downloadLost);
    connect(&_pushTimeoutTimer, &QTimer::timeout, this, &Parameters::uploadLost);
    
    _syncTimer.start();
}

Parameters::~Parameters() {

}

void Parameters::saveToFile(const QString& path) {
    QFile parameters(path);
    if(parameters.open(QFile::WriteOnly)) {
        for(auto& parameters : _parameters) {
            QString parameter = parameters->getId() + "\t" + QString::number(parameters->getValue());
        }
        parameters.close();
    }
}

void Parameters::loadFromFile() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("Parameters (*.param)");
    dialog.setViewMode(QFileDialog::Detail);
    QStringList files;
    if(dialog.exec())
        files = dialog.selectedFiles();
    QString filePath = files[0];
    QFile parameters(filePath);

    if(parameters.open(QFile::ReadOnly)) {
        
        while(!parameters.atEnd()) {
            QString line = parameters.readLine();
            QRegularExpression re("(^(\\S+?)\\s+([-+]?\\d+(?:[.,]\\d+)?(?:[eE][-+]?\\d+)?)$)");
            QRegularExpressionMatch match = re.match(line);
            if(match.hasMatch()) {
                QString name = match.captured(1);
                QString value = match.captured(2);

                auto paramIt = _parametersById.find(name);
                if(paramIt != _parametersById.end()) {
                    paramIt.value()->onValueChanged(value.toFloat());
                }
            }
        }
        parameters.close();
    }
}

void Parameters::downloadLost() {
    auto missing = _segmentMap.getMissingSegments();
    if(!missing.empty()) {
        qDebug() << "request lost";
        for(auto element : missing) {
            onRequestSingleFromVehicle(element);
        }
    } else {
        _timerPullTimeout.stop();
        emit parametersPullingCompleted();
    }
}

void Parameters::uploadLost() {

}

void Parameters::onMessage(Message msg) {
    mavlink_param_value_t param;
    mavlink_message_t* converted = msg.read<mavlink_message_t>();
    mavlink_msg_param_value_decode(converted, &param);
    _segmentMap.resize(param.param_count);
    
    auto paramIt = _parameters.find(param.param_index);
    Parameter* parameter = nullptr;
    if(paramIt != _parameters.end()) {
        parameter = paramIt.value();
    } else {
        parameter = new Parameter();
        _parameters.emplace(parameter->getIndex(), parameter);
        _parametersById.emplace(parameter->getId(), parameter);
    }
    parameter->onMessage(msg);
    emit parameterUpdated(parameter);
    if(param.param_index < _segmentMap.size()) 
        _segmentMap.segmentWritten(param.param_index);
} 

void Parameters::onErase() {
    clear();
    _segmentMap.reset();
}

void Parameters::clear() {
    for(auto& parameter : _parameters) {
        delete parameter;
    }
    _parameters.clear();
    _parametersById.clear();
}


void Parameters::onSendSingleToVehicle(uint16_t index) {
    auto paramIt = _parameters.find(index);
    if(paramIt != _parameters.end()) {
        _segmentMap.segmentErased(index);
        _pushTimeoutTimer.start();
        mavlink_message_t* msg = new mavlink_message_t{};
        mavlink_param_set_t paramSet;
        QByteArray paramNameBytes = paramIt.value()->getId().toUtf8();
        int len = qMin(paramNameBytes.size(), 16);
        strncpy(paramSet.param_id, paramNameBytes.constData(), len);
        if (len < 16) {
            paramSet.param_id[len] = '\0';
        }
        paramSet.param_type = paramIt.value()->getType();
        paramSet.param_value = paramIt.value()->getValue();
        paramSet.target_system = 0;
        paramSet.target_component = MAV_COMP_ID_AUTOPILOT1;
        mavlink_msg_param_set_encode(255, MAV_COMP_ID_MISSIONPLANNER, msg, &paramSet);
        Message msgToSend;
        msgToSend.write<mavlink_message_t>(msg);
        emit sendMessageRequest(msgToSend);
    }
}

void Parameters::onSendToVehicle() {
    for(auto paramKey : _parameters.keys()) {
        if(paramKey < _segmentMap.size())
            onSendSingleToVehicle(paramKey);
    }
}
    
void Parameters::onRequestSingleFromVehicle(uint16_t index) {
    _segmentMap.segmentErased(index);
    connect(&_timerPullTimeout, &QTimer::timeout, this, &Parameters::downloadLost);
    mavlink_message_t* msg = new mavlink_message_t{};
    mavlink_param_request_read_t paramRead;
    paramRead.param_index = index;
    mavlink_msg_param_request_read_encode(255, MAV_COMP_ID_MISSIONPLANNER, msg, &paramRead);
    Message msgToSend;
    msgToSend.write<mavlink_message_t>(msg);
    _timerPullTimeout.start();
    emit sendMessageRequest(msgToSend);
}

void Parameters::onRequestAllFromVehicle() {
    _segmentMap.reset();
    connect(&_timerPullTimeout, &QTimer::timeout, this, &Parameters::downloadLost);
    mavlink_message_t* msg = new mavlink_message_t{};
    mavlink_msg_param_request_list_pack(255, MAV_COMP_ID_MISSIONPLANNER, msg, 0, MAV_COMP_ID_AUTOPILOT1);
    Message msgToSend;
    msgToSend.write<mavlink_message_t>(msg);
    _timerPullTimeout.start();
    emit parametersPullingStarted();
    emit sendMessageRequest(msgToSend);
}

void Parameters::onConnect() {
    _pulledOnConnect = false;
}

void Parameters::onDisconnect() {
    _pulledOnConnect = true;
}