#include "mainwindow.h"

#include <fstream>

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QSplitter>

#include <Mcu.hpp>

#include "mcustate.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    this->resize(800, 600);

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

    connect(stepAction, &QAction::triggered, &McuState::instance(), &McuState::step);

    /* Step action */
    QAction* runAction = new QAction { QIcon::fromTheme("media-playback-start"), "&Run", this };
    runAction->setShortcut(QKeySequence("F5"));
    runAction->setStatusTip("Run the program");
    toolBar->addAction(runAction);

    connect(runAction, &QAction::triggered, &McuState::instance(), &McuState::run);

    this->addToolBar(Qt::TopToolBarArea, toolBar);

    /* Setup views */
    this->instructionView = new InstructionView(this);
    connect(&McuState::instance(), &McuState::stateChanged, this->instructionView, &InstructionView::update);
    connect(&McuState::instance(), &McuState::stepped, this->instructionView, &InstructionView::scroll);
    connect(this->instructionView, &InstructionView::doubleClicked, &McuState::instance(), &McuState::toggleBreakpoint);

    this->registerView = new RegisterView(this);
    connect(&McuState::instance(), &McuState::stateChanged, this->registerView, &RegisterView::update);

    this->flagView = new FlagView(this);
    connect(&McuState::instance(), &McuState::stateChanged, this->flagView, &FlagView::update);

    this->memoryView = new MemoryView(this);
    connect(&McuState::instance(), &McuState::stateChanged, this->memoryView, &MemoryView::update);

    /* Setup layout */
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    splitter->setHandleWidth(1);
    this->setCentralWidget(splitter);

    QWidget* topWidget = new QWidget;
    topWidget->setLayout(new QHBoxLayout);
    topWidget->layout()->addWidget(this->instructionView);
    topWidget->layout()->addWidget(this->registerView);
    topWidget->layout()->addWidget(this->flagView);
    splitter->addWidget(topWidget);

    QWidget* bottomWidget = new QWidget;
    bottomWidget->setLayout(new QHBoxLayout);
    bottomWidget->layout()->addWidget(this->memoryView);
    splitter->addWidget(bottomWidget);
}

MainWindow::~MainWindow()
{ }

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

    McuState::instance().load(file.readAll());
    this->update();
}
