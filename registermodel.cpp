#include "registermodel.h"

#include <QFontDatabase>

RegisterModel::RegisterModel(Mcu* mcu, QObject* parent)
    : QAbstractTableModel(parent)
    , mcu(mcu)
{ }

int RegisterModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return this->mcu->registers.size();
}

int RegisterModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant RegisterModel::data(const QModelIndex& index, int role) const {
    Q_ASSERT(index.column() == 0);
    Q_ASSERT(index.column() < this->mcu->registers.size());

    if (role == Qt::DisplayRole) {
        return { QString("%1").arg(this->mcu->registers[index.row()], 2, 16, QChar('0')).toUpper() };
    }

    if (role == Qt::FontRole) {
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPixelSize(14);
        return font;
    }

    return {};
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
