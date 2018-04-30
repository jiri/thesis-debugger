#pragma once

#include <set>

#include <QMainWindow>
#include <QTimer>

#include "instructionview.h"
#include "registerview.h"
#include "memoryview.h"
#include "flagview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    InstructionView* instructionView = nullptr;
    RegisterView* registerView = nullptr;
    FlagView* flagView = nullptr;
    MemoryView* memoryView = nullptr;

private slots:
    void update();
    void open();
};