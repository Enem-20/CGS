#include "serialconnection.h"
#include "ui_serialconnection.h"

SerialConnection::SerialConnection(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialConnection)
{
    ui->setupUi(this);
}

SerialConnection::~SerialConnection()
{
    delete ui;
}
