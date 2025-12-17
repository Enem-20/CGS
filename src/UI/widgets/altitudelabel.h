#ifndef ALTITUDELABEL_H
#define ALTITUDELABEL_H

#include <QLabel>

class AltitudeLabel : public QLabel {
public:
    AltitudeLabel(QWidget* parent = nullptr);

public slots:
    void onAltitudeUpdated(float altitude, uint64_t timestamp);
};

#endif // ALTITUDELABEL_H
