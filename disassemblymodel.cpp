#include "disassemblymodel.h"

#include <QFontDatabase>

DisassemblyModel::DisassemblyModel(std::vector<DisassembledInstruction> instructions, QObject* parent)
    : QAbstractTableModel(parent)
    , instructions(std::move(instructions))
{ }

int DisassemblyModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return this->instructions.size();
}

int DisassemblyModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 3;
}

QVariant DisassemblyModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        auto& instruction = this->instructions[index.row()];

        switch(index.column()) {
        case 0:
            return { QString("%1").arg(instruction.position, 4, 16, QChar('0')) };
        case 1: {
            QString binary_str;
            for (auto& b : instruction.binary) {
                binary_str.append(QString("%1 ").arg(b, 2, 16, QChar('0')));
            }
            return { binary_str };
        }
        case 2:
            return { instruction.print.c_str() };
        }
        return {};
    }

    if (role == Qt::FontRole) {
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPixelSize(14);
        return font;
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignLeft;
    }

    return {};
}

QVariant DisassemblyModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        Q_ASSERT(section >= 0 && section <= 3);
        switch (section) {
        case 0:
            return "Position";
        case 1:
            return "Binary";
        case 2:
            return "Instruction";
        }
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignLeft;
    }

    return {};
}
