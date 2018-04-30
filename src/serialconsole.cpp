#include "serialconsole.hpp"

#include <QVBoxLayout>

#include "mcustate.h"

SerialConsole::SerialConsole(QWidget *parent)
    : QWidget(parent)
{
    this->setLayout(new QVBoxLayout);

    this->textEdit = new QPlainTextEdit;
    this->lineEdit = new QLineEdit;

    this->layout()->addWidget(this->textEdit);
    this->layout()->addWidget(this->lineEdit);

    connect(this->lineEdit, &QLineEdit::returnPressed, [this] {
        McuState::instance().serialSend(this->lineEdit->text());
    });
}

void SerialConsole::sendChar(char chr) {
    this->textEdit->insertPlainText(QString(chr));
}
