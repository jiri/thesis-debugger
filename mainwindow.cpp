#include "mainwindow.h"

#include <fstream>

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    this->resize(800, 600);

    /* Mcu setup */
    for (auto& inst : this->mcu.disassemble()) {
        if (inst.binary[0] == 0x03) {
            this->breakpoints.insert(inst.position);
        }
    }

    /* Toolbar */
    QToolBar* toolBar = new QToolBar;
    toolBar->setMovable(false);

    /* Open action */
    QAction* openAction = new QAction { QIcon::fromTheme("document-open"), "&Open...", this };
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip("Load a binary");
    toolBar->addAction(openAction);

    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    /* Step action */
    QAction* stepAction = new QAction { QIcon::fromTheme("go-down"), "&Step", this };
    stepAction->setShortcut(QKeySequence("F9"));
    stepAction->setStatusTip("Do a step on the Mcu");
    toolBar->addAction(stepAction);

    connect(stepAction, &QAction::triggered, this, &MainWindow::step);

    /* Step action */
    QAction* runAction = new QAction { QIcon::fromTheme("media-playback-start"), "&Run", this };
    runAction->setShortcut(QKeySequence("F5"));
    runAction->setStatusTip("Run the program");
    toolBar->addAction(runAction);

    connect(runAction, &QAction::triggered, [this] { this->timer->start(); });

    this->addToolBar(Qt::TopToolBarArea, toolBar);

    /* Setup */
    QWidget* centralWidget = new QWidget;
    this->setCentralWidget(centralWidget);

    this->instructionView = new InstructionView(&this->mcu);
    this->registerView = new RegisterView(&this->mcu);
    this->memoryView = new MemoryView(&this->mcu);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    QHBoxLayout* layout1 = new QHBoxLayout;
    layout1->addWidget(this->instructionView);
    layout1->addWidget(this->registerView);
    layout->addLayout(layout1);

    QHBoxLayout* layout2 = new QHBoxLayout;
    layout2->addWidget(this->memoryView);
    layout->addLayout(layout2);

    centralWidget->setLayout(layout);

    /* 60fps Timer */
    this->timer = new QTimer;
    this->timer->setInterval(16);
//    this->timer->start();

    connect(this->timer, &QTimer::timeout, [this]() {
        // Run at 16Mhz
        for (int i = 0; i < 16000000 / 60; i++) {
            if (this->breakpoints.find(this->mcu.pc) != this->breakpoints.end()) {
                this->timer->stop();
                break;
            }
            this->mcu.step();
        }

        this->update();
    });
}

MainWindow::~MainWindow()
{ }

void MainWindow::step() {
    this->mcu.step();
    this->update();
}

void MainWindow::update() {
    this->instructionView->update();
    this->registerView->update();
    this->memoryView->update();
}

void MainWindow::open() {
    QFile file { QFileDialog::getOpenFileName(this) };

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray binary = file.readAll();

    this->mcu.load_program({ binary.begin(), binary.end() });
    this->reset();
    for (auto& inst : this->mcu.disassemble()) {
        if (inst.binary[0] == 0x03) {
            this->breakpoints.insert(inst.position);
        }
    }
    this->update();
}

void MainWindow::reset() {
//    this->mcu->reset();

    this->instructionView->reload();
    this->update();
}
