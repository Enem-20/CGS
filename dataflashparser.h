#ifndef DATAFLASHPARSER_H
#define DATAFLASHPARSER_H

#include <QObject>
#include <QVector>
#include <QByteArray>

#include <array>
#include <any>

struct LogFormatData {
    uint8_t type;
    uint8_t size;
    QString name;
    QString types;
    QString units;
    QString multipliers;
    QList<QString> columns;
    QVector<QVector<double>> values;
};

struct LogParameterData {
    QString name;
    float value;
    float defaultValue;
};

class DataFlashParser : public QObject {
    Q_OBJECT

#pragma pack(1)

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

#pragma pack()

    enum class DataFlashMessage {
        LogParameter = 32,
        LogFormatUnits = 118,
        LogFormat = 128,
    };

public:
    explicit DataFlashParser(QObject *parent = nullptr);

    void parseFile(QString path);

protected:
    QByteArray _logFileContent;
    uint32_t _cursor;

    void processFileContent();
    double parseTypeAsDouble(char type);

    QList<LogFormatData> _logData;
    QList<LogParameterData> _logParameters;

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
