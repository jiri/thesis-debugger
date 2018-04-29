#include "mcustate.h"

Q_GLOBAL_STATIC(McuState, mcuState)

McuState::McuState(QObject *parent)
    : QObject(parent)
{
    this->timer = new QTimer;
    this->timer->setInterval(16);

    connect(this->timer, &QTimer::timeout, [this] {
        // Run at 16Mhz
        for (int i = 0; i < 16000000 / 60; i++) {
            if (this->breakpoints.find(this->mcu.pc) != this->breakpoints.end()) {
                this->timer->stop();
                break;
            }
            this->mcu.step();
        }

        emit stateChanged();
    });

    this->disassembly = this->mcu.disassemble();
    emit stateChanged();
}

McuState& McuState::instance() {
    return *mcuState;
}

void McuState::load(const QByteArray& binary) {
    this->mcu.load_program({ binary.begin(), binary.end() });
    this->disassembly = this->mcu.disassemble();
    this->reset();
    emit stateChanged();
}

void McuState::step() {
    this->mcu.step();
    emit stateChanged();
}

void McuState::reset() {
    this->mcu.reset();

    this->breakpoints = {};
    for (auto& inst : this->mcu.disassemble()) {
        if (inst.binary[0] == 0x03) {
            this->breakpoints.insert(inst.position);
        }
    }

    emit stateChanged();
}

void McuState::run() {
    this->mcu.step();
    emit stateChanged();
    this->timer->start();
}

void McuState::toggleBreakpoint(const QModelIndex& index) {
    u16 position = this->disassembly[index.row()].position;
    auto it = this->breakpoints.find(position);

    if (it != this->breakpoints.end()) {
        this->breakpoints.erase(it);
    }
    else {
        this->breakpoints.insert(position);
    }

    emit stateChanged();
}
