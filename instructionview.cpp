#include "instructionview.h"

#include <QHeaderView>

#include "disassemblymodel.h"

InstructionView::InstructionView(Mcu* mcu, QWidget *parent)
    : QTableView(parent)
    , mcu(mcu)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setModel(new DisassemblyModel(mcu->disassemble(), this));

    this->horizontalHeader()->setStretchLastSection(true);

    this->verticalHeader()->hide();
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->setDefaultSectionSize(20);
}

void InstructionView::update() {
    auto disassembly = this->mcu->disassemble();
    for (size_t i = 0; i < disassembly.size(); i++) {
        if (disassembly[i].position == this->mcu->pc) {
            this->selectRow(i);
        }
    }
}
