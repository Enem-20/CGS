#ifndef ATTITUDELABEL_H
#define ATTITUDELABEL_H

#include <QLabel>

class AttitudeLabel : public QLabel {
public:
    AttitudeLabel(QWidget* parent = nullptr);

public slots:
    void onOrientationUpdated(const QVector3D& orientation, uint64_t timestamp);
};

#endif // ATTITUDELABEL_H
