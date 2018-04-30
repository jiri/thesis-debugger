#pragma once

#include <vector>
#include <set>

#include <QObject>
#include <QModelIndex>
#include <QTimer>
#include <QQueue>

#include <Mcu.hpp>

#include "serialconsole.hpp"
#include "playerview.hpp"

class McuState : public QObject
{
    Q_OBJECT
public:
    explicit McuState(QObject *parent = nullptr);
    ~McuState() override;

    Mcu mcu;

    bool running = false;
    bool breaked = false;

    std::unordered_map<u16, QString> labels;
    std::vector<DisassembledInstruction> disassembly = {};
    std::set<u16> breakpoints = {};

    static McuState& instance();

    QQueue<u8> serialQueue = {};
    SerialConsole* serialConsole = nullptr;

    u8 buttonBuffer = 0;
    PlayerView* playerView = nullptr;

private:
    QTimer* timer = nullptr;

signals:
    void stepped();
    void stateChanged();

public slots:
    void load(const QByteArray& binary);
    void loadSymbols(const QByteArray& json);
    void step();
    void reset();
    void run();
    void stop();
    void toggleBreakpointIndex(const QModelIndex& index);
    void toggleBreakpoint(u16 position);
    void serialSend(QString line);
    void showConsole();
    void showPlayer();
    void pressButton(u8 mask);
};
