#ifndef PARAMETERLIST_H
#define PARAMETERLIST_H

#include <QWidget>

namespace Ui {
class ParameterList;
}

struct __mavlink_param_value_t;
typedef __mavlink_param_value_t mavlink_param_value_t;

struct __mavlink_param_ext_value_t;
typedef __mavlink_param_ext_value_t mavlink_param_ext_value_t;

struct __mavlink_heartbeat_t;
typedef __mavlink_heartbeat_t mavlink_heartbeat_t;

struct __mavlink_param_request_list_t;
typedef __mavlink_param_request_list_t mavlink_param_request_list_t;

struct __mavlink_message;
typedef __mavlink_message mavlink_message_t;

class ParameterList : public QWidget
{
    Q_OBJECT
private:
    uint8_t _sysId = 0;
    uint8_t _compId = 0;
public:
    explicit ParameterList(QWidget *parent = nullptr);
    ~ParameterList();


signals:
    void parametersRequest(const mavlink_message_t& msg);
    void setParameterRequest(const mavlink_message_t& msg);
    void setParameterExtRequest(const mavlink_message_t& msg);
private slots:
    void on_syncVehicleWithUs_clicked();
    void on_syncUsWithVehicle_clicked();

public slots:
    void onAutopilotHeartbeat(const mavlink_message_t& msg);
    void handleMavlink(const mavlink_param_value_t& msg);
    void handleMavlink(const mavlink_param_ext_value_t& msg);

    void setSingleParameterRequested(size_t rowIndex);
    void setAllParametersRequested();
private:
    Ui::ParameterList *ui;
};

#endif // PARAMETERLIST_H
