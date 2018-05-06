#include "registermodel.h"

#include <QFontDatabase>

#include "mcustate.h"

RegisterModel::RegisterModel(QObject* parent)
    : QAbstractTableModel(parent)
{ }

int RegisterModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return McuState::instance().mcu.registers.size() + 2;
}

int RegisterModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant RegisterModel::data(const QModelIndex& index, int role) const {
    Q_ASSERT(index.isValid());

    if (role == Qt::DisplayRole) {
        u16 value = 0;
        switch (index.row()) {
            case 16:
                value = McuState::instance().mcu.pc;
                break;
            case 17:
                value = McuState::instance().mcu.sp;
                break;
            default:
                value = McuState::instance().mcu.registers[index.row()];
        }
        return { QString("%1").arg(value, 2, 16, QChar('0')).toUpper() };
    }

    if (role == Qt::FontRole) {
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPixelSize(14);
        return font;
    }

    if (role == Qt::EditRole) {
        return QVariant::String;
    }

    return {};
}

bool RegisterModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role == Qt::EditRole) {
        u16 val = value.toString().toUShort(nullptr, 16);
        switch (index.row()) {
            case 16:
                McuState::instance().mcu.pc = val;
                break;
            case 17:
                McuState::instance().mcu.sp = val;
                break;
            default:
                McuState::instance().mcu.registers[index.row()] = val;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QVariant RegisterModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal && section == 0) {
            return QString("Value");
        }
        switch (section) {
            case 16:
                return "PC";
            case 17:
                return "SP";
            default:
                return QString("R%1").arg(section);
        }

    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignLeft;
    }

    return {};
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex& index) const {
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
