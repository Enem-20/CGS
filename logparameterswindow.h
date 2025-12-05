#ifndef LOGPARAMETERSWINDOW_H
#define LOGPARAMETERSWINDOW_H

#include <QWidget>

#include "dataflashparser.h"

namespace Ui {
class LogParametersWindow;
}

class LogParametersWindow : public QWidget {
    Q_OBJECT

public:
    explicit LogParametersWindow(QWidget *parent = nullptr);
    ~LogParametersWindow();

    void showParameters(const QVector<LogParameterData>& parameters);

    static void showTemporary(const QVector<LogParameterData>& parameters, QString title = "");

private slots:
    void on_parameterFilter_textChanged(const QString &arg1);

private:
    Ui::LogParametersWindow *ui;

    void showAll();
    void hideAll();
};

#endif // LOGPARAMETERSWINDOW_H
