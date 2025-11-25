#include "parameterlist.h"
#include "ui_parameterlist.h"

#include <common/mavlink.h>

ParameterList::ParameterList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ParameterList)
{
    ui->setupUi(this);
}

ParameterList::~ParameterList()
{
    delete ui;
}

void ParameterList::on_syncWithVehicle_clicked()
{
    mavlink_message_t msg;
    mavlink_msg_param_request_list_pack(255, MAV_COMP_ID_MISSIONPLANNER, &msg, _sysId, MAV_COMP_ID_AUTOPILOT1);
    emit parametersRequest(msg);
}

void ParameterList::onAutopilotHeartbeat(const mavlink_message_t& msg) {
    _sysId = msg.sysid;
    _compId = msg.compid;
}

void ParameterList::handleMavlink(const mavlink_param_value_t& msg) {
    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(msg.param_id, Qt::MatchExactly);
    QTableWidgetItem* valueItemToSet = nullptr;
    if(!items.empty()) {
        QTableWidgetItem* parameterItemName = items[0];
        valueItemToSet = ui->tableWidget->item(parameterItemName->row(), 1);
        ui->tableWidget->setItem(parameterItemName->row(), 1, valueItemToSet);
    }
    else {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(msg.param_id));
        valueItemToSet = new QTableWidgetItem();
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, valueItemToSet);
    }

    switch (msg.param_type) {
    case MAV_PARAM_TYPE_UINT8:
        valueItemToSet->setText(QString::number(static_cast<uint8_t>(msg.param_value)));
        break;
    case MAV_PARAM_TYPE_INT8:
        valueItemToSet->setText(QString::number(static_cast<int8_t>(msg.param_value)));
        break;
    case MAV_PARAM_TYPE_UINT16:
        valueItemToSet->setText(QString::number(static_cast<uint16_t>(msg.param_value)));
        break;
    case MAV_PARAM_TYPE_INT16:
        valueItemToSet->setText(QString::number(static_cast<int16_t>(msg.param_value)));
        break;
    case MAV_PARAM_TYPE_UINT32:
        valueItemToSet->setText(QString::number(static_cast<uint32_t>(msg.param_value)));
        break;
    case MAV_PARAM_TYPE_INT32:
        valueItemToSet->setText(QString::number(static_cast<int32_t>(msg.param_value)));
        break;
    case MAV_PARAM_TYPE_REAL32:
        valueItemToSet->setText(QString::number(static_cast<float>(msg.param_value)));
        break;
    case MAV_PARAM_TYPE_REAL64:
        valueItemToSet->setText(QString::number(static_cast<double>(msg.param_value)));
        break;
    default:
        break;
    }

}

void ParameterList::handleMavlink(const mavlink_param_ext_value_t& msg) {
    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(msg.param_id, Qt::MatchExactly);
    QTableWidgetItem* valueItemToSet = nullptr;
    if(!items.empty()) {
        QTableWidgetItem* parameterItemName = items[0];
        valueItemToSet = ui->tableWidget->item(parameterItemName->row(), 1);
    }
    else {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(msg.param_id));
        valueItemToSet = new QTableWidgetItem();
    }
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, valueItemToSet);
    if(!valueItemToSet) {
        switch (msg.param_type) {
        case MAV_PARAM_TYPE_UINT8:
            valueItemToSet->setText(QString::number(*reinterpret_cast<const uint8_t*>(msg.param_value)));
            break;
        case MAV_PARAM_TYPE_INT8:
            valueItemToSet->setText(QString::number(*reinterpret_cast<const int8_t*>(msg.param_value)));
            break;
        case MAV_PARAM_TYPE_UINT16:
            valueItemToSet->setText(QString::number(*reinterpret_cast<const uint16_t*>(msg.param_value)));
            break;
        case MAV_PARAM_TYPE_INT16:
            valueItemToSet->setText(QString::number(*reinterpret_cast<const int16_t*>(msg.param_value)));
            break;
        case MAV_PARAM_TYPE_UINT32:
            valueItemToSet->setText(QString::number(*reinterpret_cast<const uint32_t*>(msg.param_value)));
            break;
        case MAV_PARAM_TYPE_INT32:
            valueItemToSet->setText(QString::number(*reinterpret_cast<const int32_t*>(msg.param_value)));
            break;
        case MAV_PARAM_TYPE_REAL32:
            valueItemToSet->setText(QString::number(*reinterpret_cast<const float*>(msg.param_value)));
            break;
        case MAV_PARAM_TYPE_REAL64:
            valueItemToSet->setText(QString::number(*reinterpret_cast<const double*>(msg.param_value)));
            break;
        default:
            break;
        }
    }
}
