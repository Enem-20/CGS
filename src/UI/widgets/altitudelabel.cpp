#include "altitudelabel.h"

AltitudeLabel::AltitudeLabel(QWidget* parent)
    : QLabel(parent) {}

void AltitudeLabel::onAltitudeUpdated(float altitude, uint64_t timestamp) {
    QString result("Alt: %1 m");
    setText(result.arg(altitude));
}
