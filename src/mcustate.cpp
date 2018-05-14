#include "mcustate.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QErrorMessage>

#include <Mcu.hpp>
#include <opcodes.hpp>

Q_GLOBAL_STATIC(McuState, mcuState)

namespace {
    void show_error(const illegal_opcode_error& e) {
        auto* error = new QErrorMessage;
        error->setWindowTitle("Error");
        error->setAttribute(Qt::WA_DeleteOnClose);
        error->showMessage(e.what());
    }

    std::string opcode_str(u8 opcode) {
        switch (opcode) {
            case NOP:   return "nop";
            case SLEEP: return "sleep";
            case BREAK: return "break";
            case SEI:   return "sei";
            case SEC:   return "sec";
            case SEZ:   return "sez";
            case CLI:   return "cli";
            case CLC:   return "clc";

            case CLZ:   return "clz";
            case ADD:   return "add";
            case ADC:   return "adc";
            case SUB:   return "sub";
            case SBC:   return "sbc";
            case INC:   return "inc";
            case DEC:   return "dec";
            case AND:   return "and";
            case OR:    return "or";
            case XOR:   return "xor";
            case CP:    return "cp ";

            case CPI:   return "cpi";
            case JMP:   return "jmp";
            case CALL:  return "call";
            case RET:   return "ret";
            case RETI:  return "reti";
            case BRC:   return "brc";
            case BRNC:  return "brnc";
            case BRZ:   return "brz";
            case BRNZ:  return "brnz";

            case MOV:   return "mov";
            case LDI:   return "ldi";
            case LD:    return "ld";
            case ST:    return "st";
            case PUSH:  return "push";
            case POP:   return "pop";
            case LPM:   return "lpm";
            case IN:    return "in";
            case OUT:   return "out";

            default:
                throw illegal_opcode_error { opcode };
        }
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

    this->disassemble();
    emit stepped();

    /* I/O handlers */
    mcu.io_handlers[0x03] = IoHandler {
            .get = [this]() -> u8 {
                if (this->serialQueue.empty()) {
                    return 0x00;
                }

                u8 tmp = this->serialQueue.front();
                this->serialQueue.pop();
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
    this->disassemble();
    this->reset();
    emit stateChanged();
}

void McuState::loadSymbols(const QByteArray& json) {
    this->labels = {};
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

    this->disassemble();
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

    this->disassemble();

    for (auto& inst : this->disassembly) {
        if (inst.binary[0] == 0x03) {
            this->breakpoints.insert(inst.position);
        }
    }

    emit stepped();
    emit stateChanged();

    if (this->serialConsole) {
        this->serialConsole->reset();
    }
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
        this->serialQueue.push(c.toLatin1());
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

void McuState::disassemble() {
    u16 pc = 0x0000;
    this->disassembly.clear();

    auto read_byte = [&]() {
        return mcu.program[pc++];
    };

    auto read_register_pair = [&]() -> std::pair<u8, u8> {
        u8 byte = read_byte();
        return { high_nibble(byte), low_nibble(byte) };
    };

    auto read_register = [&]() {
        return read_register_pair().second;
    };

    auto read_word = [&]() {
        u8 high = read_byte();
        u8 low  = read_byte();

        return static_cast<u16>(high << 8u | low);
    };

    while (true) {
        DisassembledInstruction instruction {
            .position = pc,
            .binary = {},
            .print = {},
        };

        u8 opcode = read_byte();
        instruction.binary.push_back(opcode);

        switch (opcode) {
            case NOP:
            case SLEEP:
            case BREAK:
            case SEI:
            case SEC:
            case SEZ:
            case CLI:
            case CLC:
            case CLZ:
            case RET:
            case RETI: {
                instruction.print = opcode_str(opcode);
                break;
            }
            case ADD:
            case ADC:
            case SUB:
            case SBC:
            case AND:
            case OR:
            case XOR:
            case CP:
            case MOV: {
                auto [ rDst, rSrc ] = read_register_pair();
                instruction.binary.push_back(rDst << 4u | rSrc);
                instruction.print = fmt::format("{} R{}, R{}", opcode_str(opcode), rDst, rSrc);
                break;
            }
            case LD:
            case ST:
            case LPM:
            case INC:
            case DEC:
            case PUSH:
            case POP: {
                auto rDst = read_register();
                instruction.binary.push_back(rDst);
                instruction.print = fmt::format("{} R{}", opcode_str(opcode), rDst);
                break;
            }
            case CPI:
            case LDI: {
                auto reg = read_register();
                auto val = read_byte();
                instruction.binary.push_back(reg);
                instruction.binary.push_back(val);
                instruction.print = fmt::format("{} R{}, 0x{:X}", opcode_str(opcode), reg, val);
                break;
            }
            case JMP:
            case CALL:
            case BRC:
            case BRNC:
            case BRZ:
            case BRNZ: {
                auto addr = read_word();
                instruction.binary.push_back(high_byte(addr));
                instruction.binary.push_back(low_byte(addr));
                auto addrstr = this->labels.count(addr) == 0 ? fmt::format("0x{:X}", addr) : this->labels[addr].toStdString();
                instruction.print = fmt::format("{} {}", opcode_str(opcode), addrstr);
                break;
            }
            case IN:
            case OUT: {
                auto rSrc = read_register();
                auto addr = read_byte();
                instruction.binary.push_back(rSrc);
                instruction.binary.push_back(addr);
                instruction.print = fmt::format("{} R{}, 0x{:X}", opcode_str(opcode), rSrc, addr);
                break;
            }
            default: {
                instruction.print = "ILLEGAL OPCODE";
                break;
            }
        }

        this->disassembly.push_back(instruction);

        // Overflow
        if (pc == 0x0000) {
            break;
        }
    }
}
