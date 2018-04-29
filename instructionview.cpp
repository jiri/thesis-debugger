#include "instructionview.h"

#include <QHeaderView>

#include "disassemblymodel.h"
#include "mcustate.h"

InstructionView::InstructionView(QWidget *parent)
    : QTableView(parent)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setModel(new DisassemblyModel);

    this->horizontalHeader()->setStretchLastSection(true);

    this->verticalHeader()->hide();
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->setDefaultSectionSize(20);
}

void InstructionView::scroll() {
    for (size_t i = 0; i < McuState::instance().disassembly.size(); i++) {
        if (McuState::instance().disassembly[i].position == McuState::instance().mcu.pc) {
            this->selectRow(i); // Workaround for broken this->scrollTo
        }
    }
}

void InstructionView::update() {
    this->model()->dataChanged({}, {});
}
