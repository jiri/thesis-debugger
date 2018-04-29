#pragma once

#include <QTableView>

class InstructionView : public QTableView
{
    Q_OBJECT
public:
    explicit InstructionView(QWidget* parent = nullptr);

public slots:
    void scroll();
    void update();
};
