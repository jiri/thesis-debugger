#include "flagwidget.h"

#include <QVBoxLayout>

FlagWidget::FlagWidget(Mcu* mcu, QWidget* parent)
    : QWidget(parent)
    , mcu(mcu)
{
    this->setLayout(new QVBoxLayout);

    this->zeroCheckbox = new QCheckBox("Zero");
    this->layout()->addWidget(zeroCheckbox);
    connect(zeroCheckbox, &QCheckBox::clicked, [this](bool checked) {
        this->mcu->flags.zero = checked;
    });

    this->carryCheckbox = new QCheckBox("Carry");
    this->layout()->addWidget(carryCheckbox);
    connect(carryCheckbox, &QCheckBox::clicked, [this](bool checked) {
        this->mcu->flags.carry = checked;
    });
}

void FlagWidget::update() {
    this->zeroCheckbox->setChecked(this->mcu->flags.zero);
    this->carryCheckbox->setChecked(this->mcu->flags.carry);
}
