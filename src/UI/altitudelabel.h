#ifndef ALTITUDELABEL_H
#define ALTITUDELABEL_H

#include <QLabel>

class AltitudeLabel : public QLabel {
public:
    AltitudeLabel(QWidget* parent = nullptr);

public slots:
    void onAltitudeUpdated(int32_t altitude);
};

#endif // ALTITUDELABEL_H
