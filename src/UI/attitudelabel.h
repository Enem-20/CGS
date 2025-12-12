#ifndef ATTITUDELABEL_H
#define ATTITUDELABEL_H

#include <QLabel>

class AttitudeLabel : public QLabel {
public:
    AttitudeLabel(QWidget* parent = nullptr);

public slots:
    void onOrientationUpdated(const QVector3D& orientation);
};

#endif // ATTITUDELABEL_H
