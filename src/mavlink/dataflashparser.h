#ifndef DATAFLASHPARSER_H
#define DATAFLASHPARSER_H

#include <QObject>
#include <QVector>
#include <QByteArray>

struct LogFormatData {
    uint8_t type;
    uint8_t size;
    QString name;
    QString types;
    QVector<QString> columns;
    QVector<QString> units;
    QVector<double> multipliers;
    QVector<QVector<double>> values;
};

struct LogParameterData {
    QString name;
    float value;
    float defaultValue;
};

struct LogModesData {
    QString name;
    float time;
};

class DataFlashParser : public QObject {
    Q_OBJECT

#pragma pack(push, 1)

    struct __attribute__((packed)) MessageHead {
        union {
            struct{
                uint8_t head1;
                uint8_t head2;
            };
            uint16_t head;
        };
        uint8_t msgid;
    };

    struct __attribute__((packed)) LogFormat {
        uint8_t type;
        uint8_t length;
        char name[4];
        char format[16];
        char labels[64];
    };

    struct __attribute__((packed)) LogFormatUnits {
        uint64_t timeUs;
        uint8_t formatType;
        char units[16];
        char multipliers[16];
    };

    struct __attribute__((packed)) LogParameter {
        uint64_t timeUs;
        char name[16];
        float value;
        float defaultValue;
    };

    struct __attribute__((packed)) LogMode {
        uint64_t timeUs;
        char name[16];
        float value;
        float defaultValue;
    };

#pragma pack(pop)

    enum class DataFlashMessage {
        LogParameter = 32,
        LogFormatUnits = 118,
        LogFormat = 128,
    };

public:
    explicit DataFlashParser(QObject *parent = nullptr);

    void parseFile(QString path);
    const QList<LogFormatData>& getData();
    const QList<LogParameterData>& getParameters();

protected:
    QByteArray _logFileContent;
    uint32_t _cursor;
    QList<LogFormatData> _logData;
    QList<LogParameterData> _logParameters;
    double _maxTimeValueSeconds;

    void processFileContent();

    double parseTypeAsDouble(char typeChar);
    double getMultiplier(char multiplierChar) const;
    QString getUnits(char unitsChar) const;

    template<typename T>
    T parseType() {
        void* ptr = &_logFileContent[_cursor];
        T data = *((T*)ptr);
        _cursor += sizeof(T);
        return data;
    }

signals:
};

#endif // DATAFLASHPARSER_H
