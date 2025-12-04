#include "dataflashparser.h"

#include <QDebug>
#include <QFile>

DataFlashParser::DataFlashParser(QObject *parent)
    : QObject{parent}
{}

void DataFlashParser::parseFile(QString path) {
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) return;

    _logFileContent = file.readAll();

    processFileContent();
}

void DataFlashParser::processFileContent() {
    _cursor = 0;
    _logData.clear();
    _logParameters.clear();

    while(true) {
        MessageHead messageHead = parseType<MessageHead>();

        if (messageHead.head != 0x95a3) {
            qDebug() << "Wrong message head: " << Qt::hex << Qt::showbase << messageHead.head;
            break;
        }

        switch(DataFlashMessage(messageHead.msgid)) {
            case DataFlashMessage::LogFormat: {
                LogFormat logFormat = parseType<LogFormat>();

                LogFormatData data;

                data.type = logFormat.type;
                data.size = logFormat.length;

                char nameBuffer[5] {'\0'};
                memcpy(nameBuffer, logFormat.name, 4);
                data.name = QString(nameBuffer);

                char formatBuffer[17] {'\0'};
                memcpy(formatBuffer, logFormat.format, 16);
                data.types = QString(formatBuffer);

                char labelsBuffer[65] {'\0'};
                memcpy(labelsBuffer, logFormat.labels, 64);
                QString labels = QString(labelsBuffer);
                data.columns = labels.split(",");

                data.values.resize(data.columns.size());

                _logData.push_back(data);

                break;
            }
            case DataFlashMessage::LogFormatUnits: {
                LogFormatUnits logFormatUnits = parseType<LogFormatUnits>();

                qsizetype index = -1;
                for (qsizetype i = 0; i < _logData.size(); i++) {
                    if (_logData[i].type == logFormatUnits.formatType) {
                        index = i;
                        break;
                    }
                }

                if(index == -1) {
                    qDebug() << "Failed to find data for type: " << Qt::dec << logFormatUnits.formatType;
                    break;
                }

                char unitsBuffer[17] {'\0'};
                memcpy(unitsBuffer, logFormatUnits.units, 16);
                _logData[index].units = QString(unitsBuffer);

                char multipliersBuffer[17] {'\0'};
                memcpy(multipliersBuffer, logFormatUnits.multipliers, 16);
                _logData[index].multipliers = QString(multipliersBuffer);

                break;
            }
            case DataFlashMessage::LogParameter: {
                LogParameter logParameter = parseType<LogParameter>();

                LogParameterData data;

                char nameBuffer[17] {'\0'};
                memcpy(nameBuffer, logParameter.name, 16);
                data.name = QString(nameBuffer);

                data.value = logParameter.value;
                data.defaultValue = logParameter.defaultValue;

                _logParameters.push_back(data);

                break;
            }
            default: {
                qsizetype index = -1;
                for (qsizetype i = 0; i < _logData.size(); i++) {
                    if (_logData[i].type == messageHead.msgid) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    qDebug() << "Could not find description for id: " << Qt::dec << (uint32_t)messageHead.msgid;
                    _cursor = _logFileContent.size();
                    break;
                }

                LogFormatData& data = _logData[index];

                if (data.size < 3) {
                    qDebug() << "Structure size is wrong: " << data.name;
                    _cursor = _logFileContent.size();
                    break;
                }

                if (_cursor + data.size >= _logFileContent.size()) {
                    qDebug() << "Last message is not complete: " << data.name;
                    _cursor = _logFileContent.size();
                    break;
                }

                for (qsizetype valueIndex = 0; valueIndex < data.types.size(); valueIndex++) {
                    char type = data.types[valueIndex].toLatin1();
                    double value = parseTypeAsDouble(type);
                    if (type == 'a' || type == 'n' || type == 'N' || type == 'Z') {
                        continue;
                    }
                    data.values[valueIndex].push_back(value);
                }
            }
        }

        if (_cursor == _logFileContent.size()) break;

        if (_cursor > _logFileContent.size()) {
            qDebug() << "Cursor went further then it should.";
            break;
        }
    }

    qDebug() << "parsing finished";
}

double DataFlashParser::parseTypeAsDouble(char type) {
    switch (type) {
        case 'a': {
            _cursor += 32 * sizeof(int16_t);
            return std::numeric_limits<double>::quiet_NaN();
        }
        case 'b': return static_cast<double>(parseType<int8_t>());
        case 'B': return static_cast<double>(parseType<uint8_t>());
        case 'h': return static_cast<double>(parseType<int16_t>());
        case 'H': return static_cast<double>(parseType<uint16_t>());
        case 'i': return static_cast<double>(parseType<int32_t>());
        case 'I': return static_cast<double>(parseType<uint32_t>());
        case 'f': return static_cast<double>(parseType<float>());
        case 'd': return parseType<double>();
        case 'n': {
            _cursor += 4 * sizeof(char);
            return std::numeric_limits<double>::quiet_NaN();
        }
        case 'N': {
            _cursor += 16 * sizeof(char);
            return std::numeric_limits<double>::quiet_NaN();
        }
        case 'Z': {
            _cursor += 64 * sizeof(char);
            return std::numeric_limits<double>::quiet_NaN();
        }
        case 'c': return static_cast<double>(parseType<int16_t>()) * 100.0;
        case 'C': return static_cast<double>(parseType<uint16_t>())* 100.0;
        case 'e': return static_cast<double>(parseType<int32_t>()) * 100.0;
        case 'E': return static_cast<double>(parseType<uint32_t>()) * 100.0;
        case 'L': return static_cast<double>(parseType<int32_t>()); // lat/long
        case 'M': return static_cast<double>(parseType<int8_t>()); // flight mode
        case 'q': return static_cast<double>(parseType<int64_t>());
        case 'Q': return static_cast<double>(parseType<uint64_t>());
        default:
            throw new std::exception();
    }
}
