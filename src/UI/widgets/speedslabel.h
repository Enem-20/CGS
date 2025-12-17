#ifndef SPEEDSLABEL_H
#define SPEEDSLABEL_H

#include <QLabel>

#include <QVector3D>

class SpeedsLabel : public QLabel {
public:
    SpeedsLabel(QWidget* parent = nullptr);

public slots:
    void onVelocityUpdated(const QVector3D& velocity, uint64_t timestamp);
};

#endif // SPEEDSLABEL_H
