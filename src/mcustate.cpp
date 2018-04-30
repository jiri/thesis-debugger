#include <Mcu.hpp>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QErrorMessage>

#include "mcustate.h"

Q_GLOBAL_STATIC(McuState, mcuState)

namespace {
    void show_error(const illegal_opcode_error& e) {
        auto* error = new QErrorMessage;
        error->setWindowTitle("Error");
        error->setAttribute(Qt::WA_DeleteOnClose);
        error->showMessage(e.what());
    }
}

McuState::McuState(QObject *parent)
    : QObject(parent)
{
    this->serialConsole = new SerialConsole;
    this->serialConsole->setWindowTitle("Serial console");

    this->playerView = new PlayerView;
    connect(this, &McuState::stateChanged, this->playerView, &PlayerView::update);

    this->timer = new QTimer;
    this->timer->setInterval(16);

    connect(this->timer, &QTimer::timeout, [this] {
        this->mcu.interrupts.vblank = true;

        try {
            // Run at 16Mhz
            for (int i = 0; i < 16000000 / 60; i++) {
                if (this->breakpoints.find(this->mcu.pc) != this->breakpoints.end()) {
                    this->timer->stop();
                    break;
                }
                this->mcu.step();
            }
        }
        catch (illegal_opcode_error& e) {
            show_error(e);
        }

        emit stepped();
        emit stateChanged();
    });

    this->disassembly = this->mcu.disassemble();
    emit stepped();

    /* I/O handlers */
    mcu.io_handlers[0x10] = IoHandler {
            .get = [this]() -> u8 {
                if (this->serialQueue.empty()) {
                    return 0x00;
                }

                u8 tmp = this->serialQueue.front();
                this->serialQueue.pop_front();
                return tmp;
            },
            .set = [this](u8 chr) {
                this->serialConsole->sendChar(chr);
            },
    };

    mcu.io_handlers[0x02] = IoHandler {
            .get = [this]() -> u8 {
                auto tmp = this->buttonBuffer;
                this->buttonBuffer = 0x00;
                return tmp;
            },
    };
}

McuState::~McuState() {
    delete this->serialConsole;
    delete this->playerView;
}

McuState& McuState::instance() {
    return *mcuState;
}

void McuState::load(const QByteArray& binary) {
    std::vector<u8> program(0x10000);
    for (size_t i = 0; i < std::min((size_t)binary.size(), program.size()); i++) {
        program[i] = binary.at(i);
    }
    this->mcu.load_program(program);
    this->disassembly = this->mcu.disassemble();
    this->reset();
    emit stateChanged();
}

void McuState::loadSymbols(const QByteArray& json) {
    McuState::instance().labels = {};
    QJsonParseError error {};
    QJsonObject d = QJsonDocument::fromJson(json, &error).object();

    if (error.error != QJsonParseError::NoError) {
        auto* dialog = new QErrorMessage;
        dialog->setWindowTitle("Json Error");
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->showMessage(error.errorString());
        return;
    }

    for (const auto& key : d.keys()) {
        auto position = d[key].toInt();

        if (key.contains('.')) {
            auto parts = key.split('.');
            parts[0] = "";

            if (this->labels.find(position) == this->labels.end() || this->labels[position].startsWith(QChar('.'))) {
                this->labels[position] = parts.join('.');
            }
        }
        else {
            this->labels[position] = key;
        }
    }
}

void McuState::step() {
    try {
        this->mcu.step();
        emit stepped();
        emit stateChanged();
    }
    catch (illegal_opcode_error& e) {
        show_error(e);
    }
}

void McuState::reset() {
    this->mcu.reset();

    this->labels = {};
    this->breakpoints = {};

    for (auto& inst : this->mcu.disassemble()) {
        if (inst.binary[0] == 0x03) {
            this->breakpoints.insert(inst.position);
        }
    }

    emit stepped();
    emit stateChanged();
}

void McuState::run() {
    this->step();
    this->timer->start();
}

void McuState::stop() {
    this->timer->stop();
}

void McuState::toggleBreakpointIndex(const QModelIndex& index) {
    this->toggleBreakpoint(this->disassembly[index.row()].position);
}

void McuState::toggleBreakpoint(u16 position) {
    auto it = this->breakpoints.find(position);

    if (it != this->breakpoints.end()) {
        this->breakpoints.erase(it);
    }
    else {
        this->breakpoints.insert(position);
    }

    emit stateChanged();
}

void McuState::serialSend(QString line) {
    for (QChar c : line) {
        this->serialQueue.append(c.toLatin1());
    }
    this->mcu.interrupts.serial = true;
}

void McuState::showConsole() {
    this->serialConsole->show();
}

void McuState::showPlayer() {
    this->playerView->show();
}

void McuState::pressButton(u8 mask) {
    this->mcu.interrupts.button = true;
    this->buttonBuffer |= mask;
}
