#ifndef PARAMETERLIST_H
#define PARAMETERLIST_H

#include <coroutine>

#include <QWidget>
#include <QTimer>
#include "segmentmap.h"

namespace Ui {
class ParameterList;
}

struct __mavlink_param_value_t;
typedef __mavlink_param_value_t mavlink_param_value_t;

struct __mavlink_param_ext_value_t;
typedef __mavlink_param_ext_value_t mavlink_param_ext_value_t;

struct __mavlink_param_ext_ack_t;
typedef __mavlink_param_ext_ack_t mavlink_param_ext_ack_t;


struct __mavlink_heartbeat_t;
typedef __mavlink_heartbeat_t mavlink_heartbeat_t;

struct __mavlink_param_request_list_t;
typedef __mavlink_param_request_list_t mavlink_param_request_list_t;

struct __mavlink_message;
typedef __mavlink_message mavlink_message_t;

#ifndef HAVE_ENUM_MAV_PARAM_TYPE
#define HAVE_ENUM_MAV_PARAM_TYPE
typedef enum MAV_PARAM_TYPE
{
   MAV_PARAM_TYPE_UINT8=1, /* 8-bit unsigned integer | */
   MAV_PARAM_TYPE_INT8=2, /* 8-bit signed integer | */
   MAV_PARAM_TYPE_UINT16=3, /* 16-bit unsigned integer | */
   MAV_PARAM_TYPE_INT16=4, /* 16-bit signed integer | */
   MAV_PARAM_TYPE_UINT32=5, /* 32-bit unsigned integer | */
   MAV_PARAM_TYPE_INT32=6, /* 32-bit signed integer | */
   MAV_PARAM_TYPE_UINT64=7, /* 64-bit unsigned integer | */
   MAV_PARAM_TYPE_INT64=8, /* 64-bit signed integer | */
   MAV_PARAM_TYPE_REAL32=9, /* 32-bit floating-point | */
   MAV_PARAM_TYPE_REAL64=10, /* 64-bit floating-point | */
   MAV_PARAM_TYPE_ENUM_END=11, /*  | */
} MAV_PARAM_TYPE;
#endif

class QTableWidgetItem;
class QTableWidget;

struct promise;

struct coroutine : std::coroutine_handle<promise>
{
    using promise_type = ::promise;
};

struct promise
{
    coroutine get_return_object() { return {coroutine::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
};

struct Parameter {
    QTableWidgetItem* nameItem;
    QTableWidgetItem* valueItem;
    QTableWidgetItem* typeIdItem;
    QTableWidgetItem* typeItem;

    Parameter(QTableWidgetItem* nameItem_, QTableWidgetItem* valueItem_, QTableWidgetItem* typeIdItem_, QTableWidgetItem* typeItem_);
    Parameter(QString name_, QString value_, QString typeId_ = "", QString type_ = "");
    QString name;
    QString value;
    QString typeId;
    QString type;
};

class ParameterList : public QWidget {
    Q_OBJECT
private:
    enum class State : uint8_t {
        Idle = 0,
        PullingParameters
    };

    QTimer _pullParameterTimeout;

    QString _prevEdit = "";
    coroutine _coroutineSetParameter;
    SegmentMap _segmentMap;
    QTableWidget* _buffer;
    Ui::ParameterList* ui;
    State _currentState = State::Idle;
    size_t _lastParameterSetIndex;
    uint32_t _parametersCount = 0;
    uint32_t _pulledParamsCount;
    uint8_t _sysId = 0;
    uint8_t _compId = 0;
private:
    void hideAll();
    void showAll();

    QString castParameter(MAV_PARAM_TYPE type, float value);
public:
    explicit ParameterList(QWidget *parent = nullptr);
    ~ParameterList();
signals:
    void singleParameterRequest(const mavlink_message_t& msg);
    void parametersRequest(const mavlink_message_t& msg);
    void setParameterRequest(const mavlink_message_t& msg);
    void setParameterExtRequest(const mavlink_message_t& msg);
private slots:
    void on_syncVehicleWithUs_clicked();
    void on_syncUsWithVehicle_clicked();
    void on_saveToFileButton_clicked();
    void on_loadFromFileButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

public slots:
    void onAutopilotHeartbeat(const mavlink_message_t& msg);
    void handleMavlink(const mavlink_param_value_t& msg);
    void handleMavlink(const mavlink_param_ext_value_t& msg);
    void handleMavlink(const mavlink_param_ext_ack_t& msg);

    void getSingleParamaterRequested(size_t rowIndex);
    void setSingleParameterRequested(size_t rowIndex);
    void setSingleParameterRequestedACK(size_t rowIndex);
    void setAllParametersRequested();

    void repeatParamSetRequest();
    void repeatParametersRequest();

    QString serializeParameter(size_t rowIndex);
    void saveToFile(const QString& path);
    void parameterWasSet();

    QString getParamName(size_t row) const;
    QString getParamValue(size_t row) const;
    QString getParamId(size_t row) const;
    QString getParamType(size_t row) const;

    QString getParamIdByName(const QString& paramName) const;
    QString getParamValueByName(const QString& paramName) const;

    int getParamPositionByName(const QString& paramName) const;
    int getParamPositionById(const QString& paramId) const;
    //void onParameterPulled(const mavlink_param_value_t& msg);
};

#endif // PARAMETERLIST_H
