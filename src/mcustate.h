#pragma once

#include <set>
#include <vector>
#include <queue>

#include <QObject>
#include <QModelIndex>
#include <QTimer>

#include <Mcu.hpp>

#include "serialconsole.hpp"
#include "playerview.hpp"

struct DisassembledInstruction {
    u16 position;
    std::vector<u8> binary;
    std::string print;
};

class McuState : public QObject {
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

    void disassemble();

    std::queue<u8> serialQueue = {};
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
