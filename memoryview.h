#pragma once

#include <QTableView>

#include <Mcu.hpp>

class MemoryView : public QTableView
{
    Q_OBJECT
public:
    MemoryView(Mcu* mcu, QWidget* parent = nullptr);

private:
    Mcu* mcu = nullptr;

public slots:
    void update();
};
