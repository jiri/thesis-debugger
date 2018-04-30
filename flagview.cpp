#include "flagview.h"

#include <QVBoxLayout>

#include "mcustate.h"

FlagView::FlagView(QWidget *parent)
    : QGroupBox("Flags", parent)
{
    QVBoxLayout* layout = new QVBoxLayout;

    this->zeroCheckbox = new QCheckBox("Zero");
    connect(this->zeroCheckbox, &QCheckBox::toggled, [this](bool value) {
        McuState::instance().mcu.flags.zero = value;
    });
    layout->addWidget(this->zeroCheckbox);

    this->carryCheckbox = new QCheckBox("Carry");
    connect(this->carryCheckbox, &QCheckBox::toggled, [this](bool value) {
        McuState::instance().mcu.flags.carry = value;
    });
    layout->addWidget(this->carryCheckbox);

    layout->addStretch(1);

    this->setLayout(layout);
}

void FlagView::update() {
    this->zeroCheckbox->setChecked(McuState::instance().mcu.flags.zero);
    this->carryCheckbox->setChecked(McuState::instance().mcu.flags.carry);
}
