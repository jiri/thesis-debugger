#pragma once

#include <QTableView>

#include <Mcu.hpp>

class RegisterView : public QTableView
{
    Q_OBJECT
public:
    explicit RegisterView(Mcu* mcu, QWidget* parent = nullptr);

signals:

public slots:
    void update();
};
