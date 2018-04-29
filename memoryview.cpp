#include "memoryview.h"

#include <QHeaderView>

#include "memorymodel.h"

MemoryView::MemoryView(QWidget* parent)
    : QTableView(parent)
{
    this->setModel(new MemoryModel);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->horizontalHeader()->setDefaultSectionSize(32);
    this->horizontalHeader()->setStretchLastSection(true);

    this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->setDefaultSectionSize(20);
}

void MemoryView::update() {
    this->model()->dataChanged({}, {});
}
