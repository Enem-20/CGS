#include "parameterlist.h"
#include "ui_parameterlist.h"

#include <common/mavlink.h>

ParameterList::ParameterList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ParameterList)
{
    ui->setupUi(this);
    connect(ui->parameterList, &QTableWidget::itemChanged, this, [this](QTableWidgetItem* item){
        setSingleParameterRequested(item->row());
    });
}

ParameterList::~ParameterList()
{
    delete ui;
}

void ParameterList::onAutopilotHeartbeat(const mavlink_message_t& msg) {
    _sysId = msg.sysid;
    _compId = msg.compid;
}

void ParameterList::handleMavlink(const mavlink_param_value_t& msg) {
    QList<QTableWidgetItem*> items = ui->parameterList->findItems(msg.param_id, Qt::MatchExactly);
    QTableWidgetItem* valueItemToSet = nullptr;
    ui->parameterList->blockSignals(true);
    if(!items.empty()) {
        QTableWidgetItem* parameterItemName = items[0];
        valueItemToSet = ui->parameterList->item(parameterItemName->row(), 1);
        if(!valueItemToSet) {
            valueItemToSet = new QTableWidgetItem();
            ui->parameterList->setItem(ui->parameterList->rowCount()-1, 1, valueItemToSet);
        };
        QTableWidgetItem* paramIdItem = ui->parameterList->item(parameterItemName->row(), 2);
        if(!paramIdItem) {
            paramIdItem = new QTableWidgetItem(QString::number(msg.param_index));
            paramIdItem->setFlags(paramIdItem->flags() & ~Qt::ItemIsEditable);
            ui->parameterList->setItem(parameterItemName->row(), 2, paramIdItem);
        }
        QTableWidgetItem* paramTypeItem = ui->parameterList->item(parameterItemName->row(), 3);
        if(!paramTypeItem) {
            paramTypeItem = new QTableWidgetItem(QString::number(msg.param_type));
            paramTypeItem->setFlags(paramIdItem->flags() & ~Qt::ItemIsEditable);
            ui->parameterList->setItem(parameterItemName->row(), 3, paramTypeItem);
        }
    }
    else {
        ui->parameterList->insertRow(ui->parameterList->rowCount());

        QTableWidgetItem* paramNameItem = new QTableWidgetItem(QString(msg.param_id).trimmed());
        paramNameItem->setFlags(paramNameItem->flags() & ~Qt::ItemIsEditable);
        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 0, paramNameItem);

        valueItemToSet = new QTableWidgetItem();
        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 1, valueItemToSet);

        QTableWidgetItem* paramIdItem = new QTableWidgetItem(QString::number(msg.param_index));
        paramIdItem->setFlags(paramIdItem->flags() & ~Qt::ItemIsEditable);
        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 2, paramIdItem);

        QTableWidgetItem* paramTypeItem = new QTableWidgetItem(QString::number(msg.param_type));
        paramTypeItem->setFlags(paramTypeItem->flags() & ~Qt::ItemIsEditable);
        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 3, paramTypeItem);
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
    ui->parameterList->blockSignals(false);
    //ui->tableWidget->resizeColumnsToContents();
}

void ParameterList::handleMavlink(const mavlink_param_ext_value_t& msg) {
    ui->parameterList->blockSignals(true);
    QList<QTableWidgetItem*> items = ui->parameterList->findItems(msg.param_id, Qt::MatchExactly);
    QTableWidgetItem* valueItemToSet = nullptr;
    if(!items.empty()) {
        QTableWidgetItem* parameterItemName = items[0];
        valueItemToSet = ui->parameterList->item(parameterItemName->row(), 1);
    }
    else {
        ui->parameterList->insertRow(ui->parameterList->rowCount());

        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 0, new QTableWidgetItem(msg.param_id));

        valueItemToSet = new QTableWidgetItem();
        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 1, valueItemToSet);

        QTableWidgetItem* paramIdItem = new QTableWidgetItem(QString::number(msg.param_index));
        paramIdItem->setFlags(paramIdItem->flags() & ~Qt::ItemIsEditable);
        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 2, paramIdItem);

        QTableWidgetItem* paramTypeItem = new QTableWidgetItem(QString::number(msg.param_type));
        paramTypeItem->setFlags(paramTypeItem->flags() & ~Qt::ItemIsEditable);
        //ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3, paramTypeItem);
    }

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
    ui->parameterList->blockSignals(false);
    //ui->tableWidget->resizeColumnsToContents();
}

void ParameterList::setSingleParameterRequested(size_t rowIndex) {
    QTableWidgetItem* paramNameItem = ui->parameterList->item(rowIndex, 0);
    QTableWidgetItem* paramValueItem = ui->parameterList->item(rowIndex, 1);
    QTableWidgetItem* paramTypeItem = ui->parameterList->item(rowIndex, 2);
    QTableWidgetItem* paramIndexItem = ui->parameterList->item(rowIndex, 3);
    if(paramNameItem && paramValueItem && paramTypeItem && paramIndexItem) {
        QString paramName = paramNameItem->text();
        uint8_t paramType = paramTypeItem->text().toUInt();
        uint16_t paramIndex = paramIndexItem->text().toUInt();

        mavlink_param_set_t paramSet = {0};
        paramSet.target_system = _sysId;
        paramSet.target_component = _compId;
        paramSet.param_type = paramType;
        paramSet.param_value = paramValueItem->text().toFloat();

        QByteArray paramNameBytes = paramName.toUtf8();
        int len = qMin(paramNameBytes.size(), 16);
        strncpy(paramSet.param_id, paramNameBytes.constData(), len);
        if (len < 16) {
            paramSet.param_id[len] = '\0';
        }
        mavlink_message_t msg;
        mavlink_msg_param_set_encode(255, MAV_COMP_ID_MISSIONPLANNER, &msg, &paramSet);
        emit setParameterRequest(msg);
    }
}

void ParameterList::setAllParametersRequested() {
    for(size_t i = 0; i < ui->parameterList->rowCount(); ++i) {
        setSingleParameterRequested(i);
    }
}

void ParameterList::on_syncVehicleWithUs_clicked()
{
    setAllParametersRequested();
}


void ParameterList::on_syncUsWithVehicle_clicked()
{
    mavlink_message_t msg;
    mavlink_msg_param_request_list_pack(255, MAV_COMP_ID_MISSIONPLANNER, &msg, _sysId, MAV_COMP_ID_AUTOPILOT1);
    emit parametersRequest(msg);
}

