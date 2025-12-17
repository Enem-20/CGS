#include "speedslabel.h"

SpeedsLabel::SpeedsLabel(QWidget* parent)
    : QLabel(parent) {}

void SpeedsLabel::onVelocityUpdated(const QVector3D& velocity, uint64_t timestamp) {
    QString result("FS: %1 m/s, LS: %2 m/s, VS: %3 m/s");
    setText(result
        .arg(QString::asprintf("%.2f", velocity.x()))
        .arg(QString::asprintf("%.2f", velocity.y()))
        .arg(QString::asprintf("%.2f", velocity.z()))
    );
}
