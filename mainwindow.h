#pragma once

#include <QMainWindow>
#include <QTimer>

#include <Mcu.hpp>

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
    Mcu mcu;

    InstructionView* instructionView;
    RegisterView* registerView;
    MemoryView* memoryView;
    QTimer* timer;
};
