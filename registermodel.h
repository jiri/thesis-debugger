#pragma once

#include <QAbstractTableModel>

#include <Mcu.hpp>

class RegisterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RegisterModel(Mcu* mcu, QObject* parent = nullptr);
    virtual ~RegisterModel() { }

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    Mcu* mcu = nullptr;
};
