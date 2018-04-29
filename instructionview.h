#pragma once

#include <QTableView>

class InstructionView : public QTableView
{
    Q_OBJECT
public:
    explicit InstructionView(QWidget* parent = nullptr);

private:
    void contextMenuEvent(QContextMenuEvent* event) override;

public slots:
    void scroll();
    void update();
};
