#pragma once

#include <QTableView>

#include <Mcu.hpp>

class MemoryView : public QTableView
{
    Q_OBJECT
public:
    MemoryView(QWidget* parent = nullptr);

public slots:
    void update();
};
