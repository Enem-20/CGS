#include "gcslog.h"

GCSLog::GCSLog(QWidget* parent)
    : QListWidget(parent) {}

void GCSLog::onLogUpdated(QString msg, QString severity) {
    QColor resultColor;
    if(severity == "EMERGENCY") {
        resultColor = Qt::red;
    }
    else if(severity == "ALERT") {
        resultColor = Qt::red;
    }
    else if(severity == "CRITICAL") {
        resultColor = Qt::red;
    }
    else if (severity == "ERROR") {
        resultColor = Qt::red;
    }
    else if (severity == "WARNING") {
        resultColor = Qt::yellow;
    }
    else if (severity == "NOTICE") {
        resultColor = Qt::green;
    }
    else if (severity == "INFO") {
        resultColor = Qt::black;
    }
    else if (severity == "DEBUG") {
        resultColor = Qt::blue;
    }

    QString result = QString("[%1] %2");
    QListWidgetItem* item = new QListWidgetItem(result.arg(severity).arg(msg));
    item->setForeground(resultColor);
    addItem(item);
    scrollToItem(item);
}
