#include "disassemblymodel.h"

#include <QColor>
#include <QFontDatabase>

#include "mcustate.h"

DisassemblyModel::DisassemblyModel(QObject *parent)
    : QAbstractTableModel(parent)
{ }

int DisassemblyModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return McuState::instance().disassembly.size();
}

int DisassemblyModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 3;
}

QVariant DisassemblyModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        auto& instruction = McuState::instance().disassembly[index.row()];

        switch(index.column()) {
        case 0: {
            auto it = McuState::instance().labels.find(instruction.position);
            if (it != McuState::instance().labels.end()) {
                return it->second;
            }
            else {
                return "";
            }
        }
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

    if (role == Qt::BackgroundColorRole) {
        if (McuState::instance().disassembly[index.row()].position == McuState::instance().mcu.pc) {
            return QColor(0xAA, 0xAA, 0xFF);
        }

        auto position = McuState::instance().disassembly[index.row()].position;
        if (McuState::instance().breakpoints.find(position) != McuState::instance().breakpoints.end()) {
            return QColor(0xFF, 0xAA, 0xAA);
        }
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
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            Q_ASSERT(section >= 0 && section < 3);
            switch (section) {
                case 0:
                    return "Label";
                case 1:
                    return "Binary";
                case 2:
                    return "Instruction";
            }
        }
        else if (orientation == Qt::Vertical) {
            return QString("%1").arg(McuState::instance().disassembly[section].position, 4, 16, QChar('0'));
        }
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignLeft;
    }

    return {};
}
