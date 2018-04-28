#pragma once

#include <QAbstractTableModel>

#include <Mcu.hpp>

class MemoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MemoryModel(Mcu* mcu, QObject* parent = nullptr)
        : QAbstractTableModel(parent)
        , mcu(mcu)
    { }

    virtual ~MemoryModel() { }

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    Mcu* mcu = nullptr;
};
