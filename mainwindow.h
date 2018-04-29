#pragma once

#include <set>

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
    /* Mcu */
    Mcu mcu;
    bool running = false;
    bool breaked = false;
    std::set<u16> breakpoints;

    /* Qt */
    InstructionView* instructionView;
    RegisterView* registerView;
    MemoryView* memoryView;

    QTimer* timer;

private slots:
    void step();
    void update();
    void open();
    void reset();
};
