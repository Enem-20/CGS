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

    _logFileContent.clear();
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

                LogFormatData& data = _logData[index];

                char unitsBuffer[17] {'\0'};
                memcpy(unitsBuffer, logFormatUnits.units, 16);
                QString units = QString(unitsBuffer);
                for (qsizetype unitIndex = 0; unitIndex < units.size(); unitIndex++) {
                    char unitChar = units[unitIndex].toLatin1();
                    QString unit = getUnits(unitChar);
                    data.units.push_back(unit);
                }

                char multipliersBuffer[17] {'\0'};
                memcpy(multipliersBuffer, logFormatUnits.multipliers, 16);
                QString multipliers = QString(multipliersBuffer);
                for (qsizetype multiplierIndex = 0; multiplierIndex < multipliers.size(); multiplierIndex++) {
                    char multiplierChar = multipliers[multiplierIndex].toLatin1();
                    double multiplier = getMultiplier(multiplierChar);
                    data.multipliers.push_back(multiplier);
                }

                if (data.multipliers.size() != data.units.size() || data.multipliers.size() != data.types.size()) {
                    qDebug() << "Count of types, multipliers and units doesn't match for message: " << data.name;
                }

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
                    char typeChar = data.types[valueIndex].toLatin1();
                    double value = parseTypeAsDouble(typeChar);
                    if (typeChar == 'a' || typeChar == 'n' || typeChar == 'N' || typeChar == 'Z') {
                        continue;
                    }

                    if (valueIndex < data.multipliers.size()) {
                        value *= data.multipliers[valueIndex];
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

double DataFlashParser::parseTypeAsDouble(char typeChar) {
    switch (typeChar) {
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

double DataFlashParser::getMultiplier(char multiplierChar) const {
    switch (multiplierChar) {
        case '-': return 0.0;
        case '?': return 1.0;
        case '2': return 1e2;
        case '1': return 1e1;
        case '0': return 1e0;
        case 'A': return 1e-1;
        case 'B': return 1e-2;
        case 'C': return 1e-3;
        case 'D': return 1e-4;
        case 'E': return 1e-5;
        case 'F': return 1e-6;
        case 'G': return 1e-7;
        case 'I': return 1e-9;
        case '!': return 3.6;
        case '/': return 3600.0;
        default:
            qDebug() << "Unrecognized multiplier: '" << multiplierChar << "'";
            return 1.0;
    }
}

QString DataFlashParser::getUnits(char unitsChar) const {
    switch(unitsChar) {
        case '-': return QString("");              // no units e.g. Pi, or a string
        case '?': return QString("UNKNOWN");       // Units which haven't been worked out yet....
        case 'A': return QString("A");             // Ampere
        case 'a': return QString("Ah");            // Ampere hours
        case 'd': return QString("deg");           // of the angular variety, -180 to 180
        case 'b': return QString("B");             // bytes
        case 'B': return QString("B/s");           // bytes per second
        case 'k': return QString("deg/s");         // degrees per second. Degrees are NOT SI, but is some situations more user-friendly than radians
        case 'D': return QString("deglatitude");   // degrees of latitude
        case 'e': return QString("deg/s/s");       // degrees per second per second. Degrees are NOT SI, but is some situations more user-friendly than radians
        case 'E': return QString("rad/s");         // radians per second
        case 'G': return QString("Gauss");         // Gauss is not an SI unit, but 1 tesla = 10000 gauss so a simple replacement is not possible here
        case 'h': return QString("degheading");    // 0.? to 359.?
        case 'i': return QString("A.s");           // Ampere second
        case 'J': return QString("W.s");           // Joule (Watt second)
        case 'l': return QString("l");             // litres
        case 'L': return QString("rad/s/s");       // radians per second per second
        case 'm': return QString("m");             // metres
        case 'n': return QString("m/s");           // metres per second
        case 'N': return QString("N");             // Newton
        case 'o': return QString("m/s/s");         // metres per second per second
        case 'O': return QString("degC");          // degrees Celsius. Not SI, but Kelvin is too cumbersome for most users
        case '%': return QString("%");             // percent
        case 'S': return QString("satellites");    // number of satellites
        case 's': return QString("s");             // seconds
        case 'q': return QString("rpm");           // rounds per minute. Not SI, but sometimes more intuitive than Hertz
        case 'r': return QString("rad");           // radians
        case 'U': return QString("deglongitude");  // degrees of longitude
        case 'u': return QString("ppm");           // pulses per minute
        case 'v': return QString("V");             // Volt
        case 'P': return QString("Pa");            // Pascal
        case 'w': return QString("Ohm");           // Ohm
        case 'W': return QString("Watt");          // Watt
        case 'X': return QString("W.h");           // Watt hour
        case 'y': return QString("l/s");           // litres per second
        case 'Y': return QString("us");            // pulse width modulation in microseconds
        case 'z': return QString("Hz");            // Hertz
        case '#': return QString("instance");      // (e.g.)Sensor instance number
        default:
            return QString("UNHANDLED");
    }
}
