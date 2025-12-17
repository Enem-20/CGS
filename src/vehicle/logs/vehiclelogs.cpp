#include "vehiclelogs.h"

VehicleLogs::VehicleLogs(QObject *parent)
    : ProtocolSubscriber{parent}
{}

bool VehicleLogs::changeState(State state) {
    if (state == _state) {
        qWarning() << "LogSystem changed state to the same: " << (uint32_t)_state;
        return true;
    }
    else if (_state == State::Idle || state == State::Idle) {
        State oldState = _state;
        _state = state;
        emit stateChanged(_state, oldState);
        return true;
    }
    else {
        qWarning() << "LogSystem is not allowed to change state from: " << (uint32_t)_state << " to " << (uint32_t)state;
        return false;
    }
}

LogEntry* VehicleLogs::getLogEntry(uint32_t id) {
    if (auto it = std::find_if(_logEntries.begin(), _logEntries.end(), [id](const LogEntry& entry) {
            return id == entry.id;
        }); it != _logEntries.end()) {
        return &(*it);
    }
    else {
        return nullptr;
    }
}
