#include "playerview.hpp"

#include <QGridLayout>
#include <QPixmap>
#include <QMap>

#include "mcustate.h"

namespace {
    static QMap<u8, QColor> mcuPalette {
            { 0x00, QColor { 0x00, 0x00, 0x00 } },
            { 0x01, QColor { 0x00, 0x00, 0xAA } },
            { 0x02, QColor { 0x00, 0xAA, 0x00 } },
            { 0x03, QColor { 0x00, 0xAA, 0xAA } },
            { 0x04, QColor { 0xAA, 0x00, 0x00 } },
            { 0x05, QColor { 0xAA, 0x00, 0xAA } },
            { 0x06, QColor { 0xAA, 0x55, 0x00 } },
            { 0x07, QColor { 0xAA, 0xAA, 0xAA } },
            { 0x08, QColor { 0x55, 0x55, 0x55 } },
            { 0x09, QColor { 0x55, 0x55, 0xFF } },
            { 0x0A, QColor { 0x55, 0xFF, 0x55 } },
            { 0x0B, QColor { 0x55, 0xFF, 0xFF } },
            { 0x0C, QColor { 0xFF, 0x55, 0x55 } },
            { 0x0D, QColor { 0xFF, 0x55, 0x55 } },
            { 0x0E, QColor { 0xFF, 0x55, 0xFF } },
            { 0x0F, QColor { 0xFF, 0xFF, 0xFF } },
    };
}

PlayerView::PlayerView(QWidget* parent)
    : QWidget(parent)
{
    this->setWindowTitle("Player");

    QImage buffer(160, 144, QImage::Format_RGB888);

    for (u16 y = 0; y < 144; y++) {
        for (u16 x = 0; x < 160; x++) {
            buffer.setPixel(x, y, 0);
        }
    }

    this->label = new QLabel;
    this->label->setPixmap(QPixmap::fromImage(buffer).scaled(160 * 2, 144 * 2));
    this->label->setFixedSize(160 * 2, 144 * 2);

    this->upButton = new QPushButton("U");
    connect(this->upButton, &QPushButton::pressed, [this] { McuState::instance().pressButton(1 << 0); });
    this->downButton = new QPushButton("D");
    connect(this->downButton, &QPushButton::pressed, [this] { McuState::instance().pressButton(1 << 1); });
    this->leftButton = new QPushButton("L");
    connect(this->leftButton, &QPushButton::pressed, [this] { McuState::instance().pressButton(1 << 2); });
    this->rightButton = new QPushButton("R");
    connect(this->rightButton, &QPushButton::pressed, [this] { McuState::instance().pressButton(1 << 3); });
    this->aButton = new QPushButton("A");
    connect(this->aButton, &QPushButton::pressed, [this] { McuState::instance().pressButton(1 << 4); });
    this->bButton = new QPushButton("B");
    connect(this->bButton, &QPushButton::pressed, [this] { McuState::instance().pressButton(1 << 5); });
    this->returnButton = new QPushButton(".");
    connect(this->returnButton, &QPushButton::pressed, [this] { McuState::instance().pressButton(1 << 6); });

    auto* layout = new QGridLayout;

    layout->addWidget(this->upButton, 1, 1);
    layout->addWidget(this->downButton, 3, 1);
    layout->addWidget(this->leftButton, 2, 0);
    layout->addWidget(this->rightButton, 2, 2);
    layout->addWidget(this->aButton, 2, 4);
    layout->addWidget(this->bButton, 1, 5);
    layout->addWidget(this->returnButton, 2, 1);

    layout->addWidget(this->label, 0, 0, 1, 6);

    this->setLayout(layout);
}

void PlayerView::update() {
    QImage buffer(160, 144, QImage::Format_RGB888);

    for (u16 y = 0; y < 144; y++) {
        for (u16 x = 0; x < 160; x++) {
            auto color = McuState::instance().mcu.memory[0x8000 + y * 160 + x];
            buffer.setPixel(x, y, mcuPalette[color].rgba());
        }
    }

    this->label->setPixmap(QPixmap::fromImage(buffer).scaled(160 * 2, 144 * 2));
}
