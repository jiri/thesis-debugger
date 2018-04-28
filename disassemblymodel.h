#pragma once

#include <QAbstractTableModel>

#include <Mcu.hpp>

class DisassemblyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DisassemblyModel(std::vector<DisassembledInstruction> instructions, QObject* parent = nullptr);
    virtual ~DisassemblyModel() { }

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::vector<DisassembledInstruction> instructions {};
};
