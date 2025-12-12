#ifndef SPEEDSLABEL_H
#define SPEEDSLABEL_H

#include <QLabel>

#include <QVector3D>

class SpeedsLabel : public QLabel {
public:
    SpeedsLabel(QWidget* parent = nullptr);

public slots:
    void onSpeedsUpdated(const QVector3D& speeds);
};

#endif // SPEEDSLABEL_H
