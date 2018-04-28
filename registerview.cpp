#include "registerview.h"

#include <QHeaderView>

#include "registermodel.h"

RegisterView::RegisterView(Mcu* mcu, QWidget* parent)
    : QTableView(parent)
{
    this->setModel(new RegisterModel(mcu));

    this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->setDefaultSectionSize(20);
}

void RegisterView::update() {
    this->model()->dataChanged({}, {});
}
