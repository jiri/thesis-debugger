#pragma once

#include <set>

#include <QMainWindow>
#include <QTimer>

#include "instructionview.h"
#include "registerview.h"
#include "memoryview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    InstructionView* instructionView;
    RegisterView* registerView;
    MemoryView* memoryView;

private slots:
    void update();
    void open();
};
