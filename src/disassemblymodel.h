#pragma once

#include <QAbstractTableModel>

class DisassemblyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DisassemblyModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
