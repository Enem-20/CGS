#ifndef PARAMETERLIST_H
#define PARAMETERLIST_H

#include <coroutine>

#include <QWidget>
#include <QTimer>

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

class ParameterList : public QWidget
{
    Q_OBJECT
private:
    QTimer _paramSetTimeout;
    QString _prevEdit = "";
    coroutine _coroutineSetParameter;
    QTableWidget* _buffer;
    Ui::ParameterList* ui;
    size_t _lastParameterSetIndex;
    uint8_t _sysId = 0;
    uint8_t _compId = 0;
private:
    void hideAll();
    void showAll();
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

    void on_saveToFileButton_clicked();

    void on_loadFromFileButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

public slots:
    void onAutopilotHeartbeat(const mavlink_message_t& msg);
    void handleMavlink(const mavlink_param_value_t& msg);
    void handleMavlink(const mavlink_param_ext_value_t& msg);
    void handleMavlink(const mavlink_param_ext_ack_t& msg);

    void setSingleParameterRequested(size_t rowIndex);
    coroutine setAllParametersRequested();
    void repeatParamSetRequest();

    QString serializeParameter(size_t rowIndex);
    void saveToFile(const QString& path);
    void parameterWasSet();
};

#endif // PARAMETERLIST_H
