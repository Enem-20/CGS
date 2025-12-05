#include "logparameterswindow.h"
#include "ui_logparameterswindow.h"

LogParametersWindow::LogParametersWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogParametersWindow)
{
    ui->setupUi(this);
}

LogParametersWindow::~LogParametersWindow() {
    delete ui;
}

void LogParametersWindow::showParameters(const QVector<LogParameterData>& parameters) {
    ui->parameterList->setRowCount(0);
    for (const LogParameterData& data : parameters) {
        ui->parameterList->insertRow(ui->parameterList->rowCount());
        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 0, new QTableWidgetItem(data.name));
        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 1, new QTableWidgetItem(QString::number(data.value)));
        ui->parameterList->setItem(ui->parameterList->rowCount()-1, 2, new QTableWidgetItem(QString::number(data.defaultValue)));
    }

    if (!this->isVisible()) {
        this->show();
    }
}

void LogParametersWindow::showTemporary(const QVector<LogParameterData>& parameters, QString title) {
    LogParametersWindow* tempWindow = new LogParametersWindow();
    if (title != "") {
        tempWindow->setWindowTitle(title);
    }
    tempWindow->setAttribute(Qt::WA_DeleteOnClose);
    tempWindow->showParameters(parameters);
}

void LogParametersWindow::hideAll() {
    for (qsizetype i = 0; i < ui->parameterList->rowCount(); i++) {
        if (!ui->parameterList->isRowHidden(i)) {
            ui->parameterList->hideRow(i);
        }
    }
}

void LogParametersWindow::showAll() {
    for (qsizetype i = 0; i < ui->parameterList->rowCount(); i++) {
        if (ui->parameterList->isRowHidden(i)) {
            ui->parameterList->showRow(i);
        }
    }
}

void LogParametersWindow::on_parameterFilter_textChanged(const QString &arg1) {
    if (arg1 == "") {
        showAll();
        return;
    }
    hideAll();
    QList<QTableWidgetItem*> matches = ui->parameterList->findItems(arg1, Qt::MatchRegularExpression);
    for (qsizetype i = 0; i < matches.size(); i++) {
        ui->parameterList->showRow(matches[i]->row());
    }
}
