#ifndef MODELABEL_H
#define MODELABEL_H

#include <QLabel>

#include <QJsonObject>

class ModeLabel : public QLabel {
public:
    ModeLabel(QWidget* parent = nullptr);

    void loadModes();

private:
    QJsonObject _existingModes;

public slots:
    void onModeUpdated(uint32_t mode);
};

#endif // MODELABEL_H
