#ifndef PARAMETERLIST_H
#define PARAMETERLIST_H

#include <coroutine>

#include <QWidget>
#include <QTimer>
#include "memoryManagement/segmentmap.h"

namespace Ui {
class ParameterList;
}

class Parameter;

class ParameterList : public QWidget {
    Q_OBJECT

private:
    QString _prevEdit = "";
    Ui::ParameterList* ui;

private:
    void hideAll();
    void showAll();

public:
    explicit ParameterList(QWidget *parent = nullptr);
    ~ParameterList();

signals:
    void parameterSetRequest(uint16_t index);
    void parametersSetRequest();
    void parameterReadRequest(uint16_t index);
    void parametersReadRequest();
    
private slots:
    void on_syncVehicleWithUs_clicked();
    void on_syncUsWithVehicle_clicked();
    void on_saveToFileButton_clicked();
    void on_loadFromFileButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

public slots:
    void onParameterUpdate(Parameter* parameter);
};

#endif // PARAMETERLIST_H
