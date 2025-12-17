#include "attitudelabel.h"

#include <QVector3D>

AttitudeLabel::AttitudeLabel(QWidget* parent)
    : QLabel(parent) {}

void AttitudeLabel::onOrientationUpdated(const QVector3D& orientation, uint64_t timestamp) {
    QString result("R: %1°, P: %2°, Y: %3°");
    setText(result
        .arg(QString::asprintf("%.2f", orientation.x()))
        .arg(QString::asprintf("%.2f", orientation.y()))
        .arg(QString::asprintf("%.2f", orientation.z()))
    );
}
