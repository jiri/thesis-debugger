#include "serialconsole.hpp"

#include <QVBoxLayout>

#include "mcustate.h"

SerialConsole::SerialConsole(QWidget *parent)
    : QWidget(parent)
{
    this->setLayout(new QVBoxLayout);

    this->textEdit = new QPlainTextEdit;
    this->textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    this->lineEdit = new QLineEdit;

    this->layout()->addWidget(this->textEdit);
    this->layout()->addWidget(this->lineEdit);

    connect(this->lineEdit, &QLineEdit::returnPressed, [this] {
        McuState::instance().serialSend(this->lineEdit->text());
        this->lineEdit->clear();
    });
}

void SerialConsole::sendChar(char chr) {
    if (chr == 0x00) {
        chr = '\n';
    }
    this->textEdit->insertPlainText(QString(chr));
}

void SerialConsole::reset() {
    this->lineEdit->clear();
    this->textEdit->clear();
}
