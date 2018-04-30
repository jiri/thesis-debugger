#include "instructionview.h"

#include <QContextMenuEvent>
#include <QHeaderView>
#include <QMenu>

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

    this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->setDefaultSectionSize(20);
}

void InstructionView::contextMenuEvent(QContextMenuEvent* event) {
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        const auto& instruction = McuState::instance().disassembly[index.row()];
        QMenu menu;

        menu.addAction("Toggle breakpoint", this, [instruction] {
            McuState::instance().toggleBreakpoint(instruction.position);
        });

        menu.addAction(QString("Jump to 0x%1").arg(instruction.position, 4, 16, QChar('0')), this, [instruction] {
            McuState::instance().mcu.pc = instruction.position;
        });

        menu.exec(event->globalPos());
    }
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
    this->model()->headerDataChanged(Qt::Vertical, {}, {});
}
