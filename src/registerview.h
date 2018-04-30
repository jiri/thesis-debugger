#pragma once

#include <QTableView>

#include <Mcu.hpp>

class RegisterView : public QTableView
{
    Q_OBJECT
public:
    explicit RegisterView(QWidget* parent = nullptr);

public slots:
    void update();
};
