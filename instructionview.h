#pragma once

#include <QTableView>

#include <Mcu.hpp>

class InstructionView : public QTableView
{
    Q_OBJECT
public:
    explicit InstructionView(Mcu* mcu, QWidget* parent = nullptr);

private:
    Mcu* mcu = nullptr;

signals:

public slots:
    void update();
    void reload();
};
