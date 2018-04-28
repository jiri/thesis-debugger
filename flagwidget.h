#pragma once

#include <QObject>
#include <QWidget>
#include <QCheckBox>

#include <Mcu.hpp>

class FlagWidget : public QWidget
{
    Q_OBJECT
public:
    FlagWidget(Mcu* mcu, QWidget *parent = nullptr);

private:
    Mcu* mcu = nullptr;

    QCheckBox* zeroCheckbox = nullptr;
    QCheckBox* carryCheckbox = nullptr;

signals:

public slots:
    void update();
};
