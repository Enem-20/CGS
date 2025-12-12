#ifndef GCSLOG_H
#define GCSLOG_H

#include <QListWidget>

class GCSLog : public QListWidget {
public:
    GCSLog(QWidget* parent = nullptr);

public slots:
    void onLogUpdated(QString msg, QString severity);
};

#endif // GCSLOG_H
