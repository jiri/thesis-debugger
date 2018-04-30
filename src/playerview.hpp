#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class PlayerView : public QWidget {
    Q_OBJECT
public:
    explicit PlayerView(QWidget* parent = nullptr);

private:
    QLabel* label = nullptr;

    QPushButton* upButton = nullptr;
    QPushButton* downButton = nullptr;
    QPushButton* leftButton = nullptr;
    QPushButton* rightButton = nullptr;
    QPushButton* returnButton = nullptr;
    QPushButton* aButton = nullptr;
    QPushButton* bButton = nullptr;

public slots:
    void update();
};
