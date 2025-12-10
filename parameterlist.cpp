#include "parameterlist.h"
#include "ui_parameterlist.h"

#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QFileDialog>
#include <QThread>

#include <common/mavlink.h>

Parameter::Parameter(QTableWidgetItem* nameItem_, QTableWidgetItem* valueItem_, QTableWidgetItem* typeIdItem_, QTableWidgetItem* typeItem_)
    : nameItem(nameItem_)
    , valueItem(valueItem_)
    , typeIdItem(typeIdItem_)
    , typeItem(typeIdItem_)
{
    name = nameItem ? nameItem->text() : "";
    value = valueItem ? valueItem->text() : "";
    typeId = typeIdItem ? typeIdItem->text() : "";
    type = typeItem ? typeItem->text() : "";
}

Parameter::Parameter(QString name_, QString value_, QString typeId_, QString type_)
    : name(name_)
    , value(value_)
    , typeId(typeId_)
    , type(type_)
{
    //nameItem = new QTableWidgetItem(name);
    //valueItem = new QTableWidgetItem(value);
    //typeIdItem = new QTableWidgetItem(typeId);
    //typeItem = new QTableWidgetItem(type);
}

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

QString ParameterList::castParameter(MAV_PARAM_TYPE type, float value) {
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
    _buffer = new QTableWidget();
    connect(ui->parameterList, &QTableWidget::itemChanged, this, [this](QTableWidgetItem* item){
        setSingleParameterRequested(item->row());
    });
    connect(this, &ParameterList::parametersPullingCompleted, this, &ParameterList::onParametersPullingCompleted);
    //ui->parameterList->sortItems(0);
    connect(&_heartBeatTimeout, &QTimer::timeout, this, [this]()
    {
        _isParametersInitialPulled = false;
    });
}

ParameterList::~ParameterList()
{
    delete ui;
}

void ParameterList::onAutopilotHeartbeat(const mavlink_message_t& msg) {
    _heartBeatTimeout.start(5000);
    _sysId = msg.sysid;
    _compId = msg.compid;
    emit onHeartbeatReceived();
}

void ParameterList::handleMavlink(const mavlink_param_value_t& msg) {
    _pullParameterTimeout.start(1500);
    QByteArray msgParamId(msg.param_id, 16);
    int nullIndex = msgParamId.indexOf('\0');
    if (nullIndex >= 0) {
        msgParamId.truncate(nullIndex);
    }

    _segmentMap.setBitMaskSize(msg.param_count);
    QList<QTableWidgetItem*> items = ui->parameterList->findItems(msgParamId, Qt::MatchExactly);
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

        QTableWidgetItem* paramNameItem = new QTableWidgetItem(msgParamId);
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

    QString castedParameter = castParameter(static_cast<MAV_PARAM_TYPE>(msg.param_type), msg.param_value);
    if (castedParameter != "UNKNOWN") {
        QString selfValue = getParamValueByName(msgParamId);

        valueItemToSet->setText(castedParameter);
        QString id = getParamIdByName(msgParamId);

        if ((castedParameter == selfValue) || (selfValue == "")) {
            if(msg.param_index != std::numeric_limits<uint16_t>::max())
                _segmentMap.segmentWritten(msg.param_index);
            else if(id != "miss")
                _segmentMap.segmentWritten(id.toInt());
        }

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

void ParameterList::handleMavlink(const mavlink_param_ext_ack_t& msg) {
    parameterWasSet();
}

void ParameterList::getSingleParamaterRequested(size_t rowIndex) {
    QTableWidgetItem* paramNameItem = ui->parameterList->item(rowIndex, 0);
    QTableWidgetItem* paramValueItem = ui->parameterList->item(rowIndex, 1);
    QTableWidgetItem* paramTypeItem = ui->parameterList->item(rowIndex, 2);
    QTableWidgetItem* paramIndexItem = ui->parameterList->item(rowIndex, 3);
    if(paramNameItem && paramValueItem && paramTypeItem && paramIndexItem) {
        QString paramName = paramNameItem->text();
        uint8_t paramType = paramTypeItem->text().toUInt();
        uint16_t paramIndex = paramIndexItem->text().toUInt();

        mavlink_param_request_read_t paramRead = {0};
        paramRead.target_system = _sysId;
        paramRead.target_component = _compId;

        QByteArray paramNameBytes = paramName.toUtf8();
        int len = qMin(paramNameBytes.size(), 16);
        strncpy(paramRead.param_id, paramNameBytes.constData(), len);
        if (len < 16) {
            paramRead.param_id[len] = '\0';
        }
        paramRead.param_index = getParamId(static_cast<size_t>(getParamPositionByName(paramName))).toUInt();
        mavlink_message_t msg;
        mavlink_msg_param_request_read_encode(255, MAV_COMP_ID_MISSIONPLANNER, &msg, &paramRead);

        emit singleParameterRequest(msg);
    }
}

void ParameterList::getSingleParamaterByIdRequested(size_t paramId) {
    mavlink_param_request_read_t paramRead = {0};
    paramRead.target_system = _sysId;
    paramRead.target_component = _compId;
    paramRead.param_index = static_cast<int16_t>(paramId);
    mavlink_message_t msg;
    mavlink_msg_param_request_read_encode(255, MAV_COMP_ID_MISSIONPLANNER, &msg, &paramRead);

    emit singleParameterRequest(msg);
}

void ParameterList::setSingleParameterRequested(size_t rowIndex) {
    QTableWidgetItem* paramNameItem = ui->parameterList->item(rowIndex, 0);
    QTableWidgetItem* paramValueItem = ui->parameterList->item(rowIndex, 1);
    QTableWidgetItem* paramIndexItem = ui->parameterList->item(rowIndex, 2);
    QTableWidgetItem* paramTypeItem = ui->parameterList->item(rowIndex, 3);
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

void ParameterList::setSingleParameterRequestedACK(size_t rowIndex) {
    QTableWidgetItem* paramNameItem = ui->parameterList->item(rowIndex, 0);
    QTableWidgetItem* paramValueItem = ui->parameterList->item(rowIndex, 1);
    QTableWidgetItem* paramTypeItem = ui->parameterList->item(rowIndex, 2);
    QTableWidgetItem* paramIndexItem = ui->parameterList->item(rowIndex, 3);
    if(paramNameItem && paramValueItem && paramTypeItem && paramIndexItem) {
        QString paramName = paramNameItem->text();
        uint8_t paramType = paramTypeItem->text().toUInt();
        uint16_t paramIndex = paramIndexItem->text().toUInt();

        mavlink_param_ext_set_t paramSet = {0};
        paramSet.target_system = _sysId;
        paramSet.target_component = _compId;
        paramSet.param_type = paramType;
        float param = paramValueItem->text().toFloat();
        std::memcpy(paramSet.param_value, &param, sizeof(param));

        QByteArray paramNameBytes = paramName.toUtf8();
        int len = qMin(paramNameBytes.size(), 16);
        strncpy(paramSet.param_id, paramNameBytes.constData(), len);
        if (len < 16) {
            paramSet.param_id[len] = '\0';
        }
        mavlink_message_t msg;
        mavlink_msg_param_ext_set_encode(255, MAV_COMP_ID_MISSIONPLANNER, &msg, &paramSet);

        _lastParameterSetIndex = rowIndex;

        emit setParameterRequest(msg);
    }
}

void ParameterList::setAllParametersRequested() {
    _segmentMap.reset();
    _pullParameterTimeout.start(1500);
    connect(&_pullParameterTimeout, &QTimer::timeout, this, &ParameterList::repeatParamSetRequest);
    for (size_t i = 0; i < ui->parameterList->rowCount(); ++i) {
        setSingleParameterRequested(i);
        //QThread::usleep(1000);
    }
}

void ParameterList::repeatParamSetRequest() {
    qDebug() << "repeating set parameters";
    QList<size_t> missingSegments = _segmentMap.getMissingSegments();
    if(!missingSegments.size()) disconnect(&_pullParameterTimeout, &QTimer::timeout, this, &ParameterList::repeatParamSetRequest);
    for(size_t i = 0; i < missingSegments.size(); ++i) {
        QList<QTableWidgetItem*> items = ui->parameterList->findItems(QString::number(missingSegments[i]), Qt::MatchExactly);
        if(items.size()) {
            QTableWidgetItem* foundItem = nullptr;
            for(size_t i = 0; i < items.size(); ++i) {
                if(items[i]->column() == 2) foundItem = items[i];
            }
            if(foundItem)
                setSingleParameterRequested(foundItem->row());
        }
    }

}

void ParameterList::repeatParametersRequest() {
    qDebug() << "repeating request parameters";
    QList<size_t> missingSegments = _segmentMap.getMissingSegments();
    if (!missingSegments.size()) {
        qDebug() << "Finished loading parameters from vehilce";
        emit parametersPullingCompleted();
    }

    for (size_t i = 0; i < missingSegments.size(); ++i) {
        QList<QTableWidgetItem*> items = ui->parameterList->findItems(QString::number(missingSegments[i]), Qt::MatchExactly);
        getSingleParamaterByIdRequested(missingSegments[i]);
        //if (items.size()) {
        //    QTableWidgetItem* foundItem = items[0];
        //    getSingleParamaterRequested(foundItem->row());
        //}
        //else {

        //}
        // else {
        //     mavlink_message_t msg;
        //     mavlink_msg_param_request_list_pack(255, MAV_COMP_ID_MISSIONPLANNER, &msg, _sysId, MAV_COMP_ID_AUTOPILOT1);
        //     qDebug() << "Missing some parameter names. Repeating request for all.";
        //     emit parametersRequest(msg);
        //     break;
        // }
    }
}

QString ParameterList::serializeParameter(size_t rowIndex) {
    Parameter parameter(
        ui->parameterList->item(rowIndex, 0),
        ui->parameterList->item(rowIndex, 1),
        ui->parameterList->item(rowIndex, 2),
        ui->parameterList->item(rowIndex, 3)
    );
    return parameter.name + "\t" + parameter.value;
}

void ParameterList::saveToFile(const QString& path) {
    QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Cannot open file for writing:" << file.errorString();
            return;
        }

        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);

        for(int i = 0; i < ui->parameterList->rowCount(); ++i) {
            QString line = serializeParameter(i);
            if (!line.isEmpty()) {
                stream << line << "\n";
            }

            if (stream.status() != QTextStream::Ok) {
                qDebug() << "Stream error at parameter" << i << ":" << stream.status();
                break;
            }

            if (i % 100 == 0) {
                stream.flush();
                if (file.error() != QFile::NoError) {
                    qDebug() << "File error at parameter" << i << ":" << file.errorString();
                    break;
                }
            }
        }

        stream.flush();
        file.close();

        qDebug() << "Total parameters processed:" << ui->parameterList->rowCount();
        qDebug() << "File size:" << file.size() << "bytes";
}

void ParameterList::parameterWasSet() {
    _coroutineSetParameter.resume();
}

QString ParameterList::getParamName(size_t row) const {
    QTableWidgetItem* item = ui->parameterList->item(row, 0);
    return (item) ? item->text() : "miss";
}

QString ParameterList::getParamValue(size_t row) const {
    QTableWidgetItem* item = ui->parameterList->item(row, 1);
    return (item) ? item->text() : "miss";
}

QString ParameterList::getParamId(size_t row) const {
    QTableWidgetItem* item = ui->parameterList->item(row, 2);
    return (item) ? item->text() : "miss";
}

QString ParameterList::getParamType(size_t row) const {
    QTableWidgetItem* item = ui->parameterList->item(row, 3);
    return (item) ? item->text() : "miss";
}

QString ParameterList::getParamIdByName(const QString& paramName) const {
    int paramRowPosition = getParamPositionByName(paramName);
    return (paramRowPosition != -1) ? ui->parameterList->item(paramRowPosition, 2)->text() : "miss";
}

QString ParameterList::getParamValueByName(const QString& paramName) const {
    int paramRowPosition = getParamPositionByName(paramName);
    return (paramRowPosition != -1) ? ui->parameterList->item(paramRowPosition, 1)->text() : "miss";
}

int ParameterList::getParamPositionByName(const QString& paramName) const {
    QList<QTableWidgetItem*> items = ui->parameterList->findItems(paramName, Qt::MatchExactly);
    return items.size() ? items[0]->row() : -1;
}

int ParameterList::getParamPositionById(const QString& paramId) const {
    QList<QTableWidgetItem*> items = ui->parameterList->findItems(paramId, Qt::MatchExactly);
    return items.size() ? items[0]->row() : -1;
}

void ParameterList::onParametersPullingCompleted() {
    _isParametersInitialPulled = true;
    disconnect(&_pullParameterTimeout, &QTimer::timeout, this, &ParameterList::repeatParametersRequest);
}

void ParameterList::onHeartbeatReceived() {
    if(!_isParametersInitialPulled) {
        on_syncUsWithVehicle_clicked();
        _isParametersInitialPulled = true;
    }
}

void ParameterList::on_syncVehicleWithUs_clicked() {
    setAllParametersRequested();
}


void ParameterList::on_syncUsWithVehicle_clicked() {
    ui->parameterList->setRowCount(0);
    _segmentMap.reset();
    connect(&_pullParameterTimeout, &QTimer::timeout, this, &ParameterList::repeatParametersRequest);
    mavlink_message_t msg;
    mavlink_msg_param_request_list_pack(255, MAV_COMP_ID_MISSIONPLANNER, &msg, _sysId, MAV_COMP_ID_AUTOPILOT1);
    emit parametersRequest(msg);
}

void ParameterList::on_saveToFileButton_clicked() {
    ui->parameterList->sortItems(0);
    saveToFile("/home/szamaro/Projects/CGS/parameterSet.param");
}


void ParameterList::on_loadFromFileButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/jana", tr("Image Files (*.parm *.param)"));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    if (file.isOpen()) {
        ui->parameterList->blockSignals(true);
        while (file.isReadable()) {
            QString line = file.readLine();
            line = line.trimmed();
            if (line.size() < 1) break;
            QStringList list = line.split(QRegularExpression("[\\s,]+"));

            Parameter parameter(list[0], list[1]);
            QList<QTableWidgetItem*> items = ui->parameterList->findItems(parameter.name, Qt::MatchExactly);
            if (!items.empty()) {
                ui->parameterList->item(items[0]->row(), 1)->setText(parameter.value);
            }
            else {
                ui->parameterList->insertRow(ui->parameterList->rowCount());
                QTableWidgetItem* nameItem = new QTableWidgetItem(parameter.name);
                ui->parameterList->setItem(ui->parameterList->rowCount() - 1, 0, nameItem);
                ui->parameterList->setItem(nameItem->row(), 1, new QTableWidgetItem(parameter.value));
            }
        }
        ui->parameterList->blockSignals(false);
    }
}

void ParameterList::on_lineEdit_textChanged(const QString &arg1) {
    if (arg1 == "") { showAll(); return; }
    hideAll();
    QList<QTableWidgetItem*> matches = ui->parameterList->findItems(arg1, Qt::MatchRegularExpression);
    for (size_t i = 0; i < matches.size(); ++i) {
        ui->parameterList->showRow(matches[i]->row());
    }
}

void ParameterList::onActiveDeviceChanged(QStringView deviceName) {
    ui->parameterList->setRowCount(0);
    _segmentMap.reset();
    mavlink_message_t msg;
    mavlink_msg_param_request_list_pack(255, MAV_COMP_ID_MISSIONPLANNER, &msg, _sysId, MAV_COMP_ID_AUTOPILOT1);
    emit parametersRequest(msg);
}
