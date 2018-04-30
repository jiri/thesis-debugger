#pragma once

#include <QGroupBox>
#include <QCheckBox>

class FlagView : public QGroupBox {
    Q_OBJECT
public:
    explicit FlagView(QWidget *parent = nullptr);

private:
    QCheckBox* zeroCheckbox = nullptr;
    QCheckBox* carryCheckbox = nullptr;

public slots:
    void update();
};
