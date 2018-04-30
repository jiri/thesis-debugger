#pragma once

#include <vector>
#include <set>

#include <QObject>
#include <QModelIndex>
#include <QTimer>
#include <QQueue>

#include <Mcu.hpp>

#include "serialconsole.hpp"

class McuState : public QObject
{
    Q_OBJECT
public:
    explicit McuState(QObject *parent = nullptr);
    ~McuState() override;

    Mcu mcu;

    bool running = false;
    bool breaked = false;

    std::vector<DisassembledInstruction> disassembly = {};
    std::set<u16> breakpoints = {};

    static McuState& instance();

    QQueue<u8> serialQueue = {};
    SerialConsole* serialConsole = nullptr;

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
    void stop();
    void toggleBreakpointIndex(const QModelIndex& index);
    void toggleBreakpoint(u16 position);
    void serialSend(QString line);
    void showConsole();
};
