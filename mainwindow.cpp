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

    /* Toolbar */
    QToolBar* toolBar = new QToolBar;
    toolBar->setMovable(false);

    /* Open action */
    QAction *openAction = new QAction { QIcon::fromTheme("document-open"), "&Open...", this };
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Load a binary"));

    connect(openAction, &QAction::triggered, this, &MainWindow::open);

//    fileMenu->addAction(openAct);
    toolBar->addAction(openAction);

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
    this->timer->start();

    connect(this->timer, &QTimer::timeout, [this]() {
        // Run at 16Mhz
        for (int i = 0; i < 266667; i++) {
            this->mcu.step();
        }

        this->instructionView->update();
        this->registerView->update();
        this->memoryView->update();

        if (this->mcu.stopped) {
            this->running = false;
            this->timer->stop();
        }
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

    this->instructionView->reload();
    this->update();
}
