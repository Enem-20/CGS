#include "modelabel.h"

#include <QFile>
#include <QJsonDocument>

ModeLabel::ModeLabel(QWidget* parent)
    : QLabel(parent) 
{
    loadModes();
}

void ModeLabel::loadModes() {
    QFile file("modes.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open file: " << "modes.json";
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "modes.json parse error: " << error.errorString();
        return;
    }

    _existingModes = doc.object();
}

void ModeLabel::onModeUpdated(uint32_t mode) {
    QString stringifiedCustomMode = QString::number(mode);
    if (_existingModes.contains(stringifiedCustomMode)) {
        setText(_existingModes[stringifiedCustomMode].toString());
    }
    else {
        setText("UNKNOWN");
    }
}
