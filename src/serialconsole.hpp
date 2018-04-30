#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>

class SerialConsole : public QWidget {
    Q_OBJECT
public:
    explicit SerialConsole(QWidget* parent = nullptr);

private:
    QPlainTextEdit* textEdit = nullptr;
    QLineEdit* lineEdit = nullptr;

public slots:
    void sendChar(char chr);
};
