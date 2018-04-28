#include "memoryview.h"

#include <QHeaderView>

#include "memorymodel.h"

MemoryView::MemoryView(Mcu* mcu, QWidget* parent)
    : QTableView(parent)
    , mcu(mcu)
{
    this->setModel(new MemoryModel(mcu));

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->horizontalHeader()->setDefaultSectionSize(32);
    this->horizontalHeader()->setStretchLastSection(true);

    this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->setDefaultSectionSize(20);
}

void MemoryView::update() {
    this->model()->dataChanged({}, {});
}
