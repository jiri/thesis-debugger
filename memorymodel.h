#pragma once

#include <QAbstractTableModel>

class MemoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MemoryModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
