#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <QWidget>

namespace Ui {
class SerialConnection;
}

class SerialConnection : public QWidget
{
    Q_OBJECT

public:
    explicit SerialConnection(QWidget *parent = nullptr);
    ~SerialConnection();

private:
    Ui::SerialConnection *ui;
};

#endif // SERIALCONNECTION_H
