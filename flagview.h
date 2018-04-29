#pragma once

#include <QWidget>
#include <QCheckBox>

class FlagView : public QWidget {
    Q_OBJECT
public:
    explicit FlagView(QWidget *parent = nullptr);

private:
    QCheckBox* zeroCheckbox = nullptr;
    QCheckBox* carryCheckbox = nullptr;

public slots:
    void update();
};
