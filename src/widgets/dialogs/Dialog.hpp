#pragma once

#include <QtCore>
#include <QtWidgets>

#include "interfaces/ISavableLoadable.hpp"

class Dialog : public QDialog, public ISavableLoadable
{
    Q_OBJECT

public:
    Dialog(QWidget *parent);
    ~Dialog();

    void closeEvent(QCloseEvent *event) override;

protected:
    void save() override = 0;
    void load() override = 0;
};
