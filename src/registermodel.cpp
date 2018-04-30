#include "registermodel.h"

#include <QFontDatabase>

#include "mcustate.h"

RegisterModel::RegisterModel(QObject* parent)
    : QAbstractTableModel(parent)
{ }

int RegisterModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return McuState::instance().mcu.registers.size();
}

int RegisterModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant RegisterModel::data(const QModelIndex& index, int role) const {
    Q_ASSERT(index.column() == 0);
    Q_ASSERT(index.column() < McuState::instance().mcu.registers.size());

    if (role == Qt::DisplayRole) {
        return { QString("%1").arg(McuState::instance().mcu.registers[index.row()], 2, 16, QChar('0')).toUpper() };
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
        McuState::instance().mcu.registers[index.row()] = value.toString().toUShort(nullptr, 16);
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
        return QString("R%1").arg(section);
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignLeft;
    }

    return {};
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex& index) const {
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
