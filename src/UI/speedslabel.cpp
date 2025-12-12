#include "speedslabel.h"

SpeedsLabel::SpeedsLabel(QWidget* parent)
    : QLabel(parent) {}

void SpeedsLabel::onSpeedsUpdated(const QVector3D& speeds) {
    QString result("FS: %1 m/s, LS: %2 m/s, VS: %3 m/s");
    setText(result
        .arg(QString::asprintf("%.2f", speeds.x()))
        .arg(QString::asprintf("%.2f", speeds.y()))
        .arg(QString::asprintf("%.2f", speeds.z()))
    );
}
