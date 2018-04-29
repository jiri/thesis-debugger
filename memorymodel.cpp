#include "memorymodel.h"

#include <QFontDatabase>

#include "mcustate.h"

MemoryModel::MemoryModel(QObject *parent)
    : QAbstractTableModel(parent)
{ }

int MemoryModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 0x1000;
}

int MemoryModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 0x10 + 1;
}

QVariant MemoryModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        if (index.column() == 16) {
            QString ret;
            for (u16 i = 0; i < 0x10; i++) {
                char chr = McuState::instance().mcu.memory[index.row() * 0x10 + i];
                ret.append(isprint(chr) ? chr : '.');
            }
            return ret;
        }
        auto byte = McuState::instance().mcu.memory[index.row() * 0x10 + index.column()];
        return { QString("%1").arg(byte, 2, 16, QChar('0')).toUpper() };
    }

    if (role == Qt::FontRole) {
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPixelSize(14);
        return font;
    }

    if (role == Qt::TextAlignmentRole) {
        return index.column() == 16 ? Qt::AlignLeft : Qt::AlignCenter;
    }

    return {};
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            if (section == 16) {
                return "ASCII";
            }
            return QString("%1").arg(section, 2, 16, QChar('0')).toUpper();
        }
        else if (orientation == Qt::Vertical) {
            return QString("%1").arg(section * 0x10, 4, 16, QChar('0')).toUpper();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (orientation == Qt::Horizontal) {
            return section == 16 ? Qt::AlignLeft : Qt::AlignCenter;
        }
        else if (orientation == Qt::Vertical) {
            return Qt::AlignRight;
        }
    }

    return {};
}
