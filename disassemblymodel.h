#pragma once

#include <QAbstractTableModel>

#include <Mcu.hpp>

class DisassemblyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DisassemblyModel(Mcu* mcu, QObject* parent = nullptr);
    virtual ~DisassemblyModel() { }

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    Mcu* mcu = nullptr;
    std::vector<DisassembledInstruction> instructions {};

public slots:
    void reload();
};
