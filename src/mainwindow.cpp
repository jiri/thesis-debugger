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
    this->setWindowTitle("Debugger");
    this->resize(800, 600);

    /* Toolbar */
    auto* toolBar = new QToolBar;
    toolBar->setMovable(false);

    toolBar->addAction(QIcon::fromTheme("document-open"), "&Open...", this, &MainWindow::open);
    toolBar->actions().last()->setShortcuts(QKeySequence::Open);

    toolBar->addAction(QIcon::fromTheme("document-open"), "&Load symfile...", this, &MainWindow::openSymfile);

    toolBar->addAction(QIcon::fromTheme("view-refresh"), "&Reset", &McuState::instance(), &McuState::reset);
    toolBar->actions().last()->setShortcut(QKeySequence("F1"));

    toolBar->addAction(QIcon::fromTheme("go-down"), "&Step", &McuState::instance(), &McuState::step);
    toolBar->actions().last()->setShortcut(QKeySequence("F9"));

    toolBar->addAction(QIcon::fromTheme("media-playback-start"), "&Run", &McuState::instance(), &McuState::run);
    toolBar->actions().last()->setShortcut(QKeySequence("F5"));

    toolBar->addAction(QIcon::fromTheme("media-playback-stop"), "&Stop", &McuState::instance(), &McuState::stop);
    toolBar->actions().last()->setShortcut(QKeySequence("F6"));

    toolBar->addAction(QIcon::fromTheme("utilities-terminal"), "&Console", &McuState::instance(), &McuState::showConsole);

    toolBar->addAction(QIcon::fromTheme("utilities-terminal"), "&Player", &McuState::instance(), &McuState::showPlayer);

    this->addToolBar(Qt::TopToolBarArea, toolBar);

    /* Setup views */
    this->instructionView = new InstructionView(this);
    connect(&McuState::instance(), &McuState::stateChanged, this->instructionView, &InstructionView::update);
    connect(&McuState::instance(), &McuState::stepped, this->instructionView, &InstructionView::scroll);
    connect(this->instructionView, &InstructionView::doubleClicked, &McuState::instance(), &McuState::toggleBreakpointIndex);

    this->registerView = new RegisterView(this);
    connect(&McuState::instance(), &McuState::stateChanged, this->registerView, &RegisterView::update);

    this->flagView = new FlagView(this);
    connect(&McuState::instance(), &McuState::stateChanged, this->flagView, &FlagView::update);

    this->memoryView = new MemoryView(this);
    connect(&McuState::instance(), &McuState::stateChanged, this->memoryView, &MemoryView::update);

    /* Setup layout */
    auto* splitter = new QSplitter(Qt::Vertical);
    splitter->setHandleWidth(1);
    this->setCentralWidget(splitter);

    auto* topWidget = new QWidget;
    topWidget->setLayout(new QHBoxLayout);
    topWidget->layout()->addWidget(this->instructionView);
    topWidget->layout()->addWidget(this->registerView);
    topWidget->layout()->addWidget(this->flagView);
    splitter->addWidget(topWidget);

    auto* bottomWidget = new QWidget;
    bottomWidget->setLayout(new QHBoxLayout);
    bottomWidget->layout()->addWidget(this->memoryView);
    splitter->addWidget(bottomWidget);
}

MainWindow::~MainWindow() = default;

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

void MainWindow::openSymfile() {
    QFile file { QFileDialog::getOpenFileName(this) };

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    McuState::instance().loadSymbols(file.readAll());
    this->update();
}
