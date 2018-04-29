#include "registerview.h"

#include <QHeaderView>

#include "registermodel.h"

RegisterView::RegisterView(QWidget* parent)
    : QTableView(parent)
{
    this->setModel(new RegisterModel);

    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->setDefaultSectionSize(20);

    this->setMaximumWidth(120);
}

void RegisterView::update() {
    this->model()->dataChanged({}, {});
}
