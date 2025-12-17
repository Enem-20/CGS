#include "parameterlist.h"
#include "ui_parameterlist.h"

#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QFileDialog>
#include <QThread>

#include <common/mavlink.h>

#include "vehicle/parameters/Parameter.h"

void ParameterList::hideAll() {
    for(size_t i = 0; i < ui->parameterList->rowCount(); ++i) {
        if(!ui->parameterList->isRowHidden(i))
            ui->parameterList->hideRow(i);
    }
}

void ParameterList::showAll() {
    for(size_t i = 0; i < ui->parameterList->rowCount(); ++i) {
        if(ui->parameterList->isRowHidden(i))
            ui->parameterList->showRow(i);
    }
}

QString castParameter(MAV_PARAM_TYPE type, float value) {
    switch (type) {
    case MAV_PARAM_TYPE_UINT8:
        return QString::number(static_cast<uint8_t>(value));
        break;
    case MAV_PARAM_TYPE_INT8:
        return QString::number(static_cast<int8_t>(value));
        break;
    case MAV_PARAM_TYPE_UINT16:
        return QString::number(static_cast<uint16_t>(value));
        break;
    case MAV_PARAM_TYPE_INT16:
        return QString::number(static_cast<int16_t>(value));
        break;
    case MAV_PARAM_TYPE_UINT32:
        return QString::number(static_cast<uint32_t>(value));
        break;
    case MAV_PARAM_TYPE_INT32:
        return QString::number(static_cast<int32_t>(value));
        break;
    case MAV_PARAM_TYPE_REAL32:
        return QString::number(static_cast<float>(value));
        break;
    case MAV_PARAM_TYPE_REAL64:
        return QString::number(static_cast<double>(value));
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

ParameterList::ParameterList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ParameterList)
{
    ui->setupUi(this);
    connect(ui->parameterList, &QTableWidget::itemChanged, this, [this](QTableWidgetItem* item){
        emit parameterSetRequest(ui->parameterList->item(item->row(), 2)->text().toUInt());
    });
}

ParameterList::~ParameterList()
{
    delete ui;
}

void ParameterList::on_syncVehicleWithUs_clicked() {
    emit parametersSetRequest();
}


void ParameterList::on_syncUsWithVehicle_clicked() {
    emit parametersReadRequest();
}

void ParameterList::on_saveToFileButton_clicked() {
    ui->parameterList->sortItems(0);
    //saveToFile("/home/szamaro/Projects/CGS/parameterSet.param");
}


void ParameterList::on_loadFromFileButton_clicked() {
    
}

void ParameterList::on_lineEdit_textChanged(const QString &arg1) {
    if (arg1 == "") { showAll(); return; }
    hideAll();
    QList<QTableWidgetItem*> matches = ui->parameterList->findItems(arg1, Qt::MatchRegularExpression);
    for (size_t i = 0; i < matches.size(); ++i) {
        ui->parameterList->showRow(matches[i]->row());
    }
}

void ParameterList::onParameterUpdate(Parameter* parameter) {
    auto items = ui->parameterList->findItems(parameter->getId(), Qt::MatchExactly);
    size_t rowIndex = 0;
    QTableWidgetItem* nameItem = nullptr;
    QTableWidgetItem* valueItem = nullptr;
    QTableWidgetItem* idItem = nullptr;
    QTableWidgetItem* typeItem = nullptr;
    if(items.empty()) {
        nameItem = new QTableWidgetItem(parameter->getId());
        valueItem = new QTableWidgetItem(castParameter(static_cast<MAV_PARAM_TYPE>(parameter->getType()), parameter->getValue()));
        idItem = new QTableWidgetItem(QString::number(parameter->getIndex()));
        typeItem = new QTableWidgetItem(QString::number(parameter->getType()));
        rowIndex = ui->parameterList->rowCount();
    } else {
        rowIndex = items[0]->row();
        nameItem = ui->parameterList->item(rowIndex, 0);
        valueItem = ui->parameterList->item(rowIndex, 1);
        idItem = ui->parameterList->item(rowIndex, 2);
        typeItem = ui->parameterList->item(rowIndex, 3);
    }

    ui->parameterList->insertRow(rowIndex);
    ui->parameterList->setItem(rowIndex, 0, nameItem);
    ui->parameterList->setItem(rowIndex, 1, valueItem);
    ui->parameterList->setItem(rowIndex, 2, idItem);
    ui->parameterList->setItem(rowIndex, 3, typeItem);
}