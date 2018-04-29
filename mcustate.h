#pragma once

#include <vector>
#include <set>

#include <QObject>
#include <QModelIndex>
#include <QTimer>

#include <Mcu.hpp>

class McuState : public QObject
{
    Q_OBJECT
public:
    explicit McuState(QObject *parent = nullptr);

    Mcu mcu;

    bool running = false;
    bool breaked = false;

    std::vector<DisassembledInstruction> disassembly = {};
    std::set<u16> breakpoints = {};

    static McuState& instance();

private:
    QTimer* timer = nullptr;

signals:
    void stepped();
    void stateChanged();

public slots:
    void load(const QByteArray& binary);
    void step();
    void reset();
    void run();
    void toggleBreakpoint(const QModelIndex& index);
};
