#include "mainwindow.h"

#include <fstream>

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    this->resize(800, 600);

    /* Read whole binary file */
    std::ifstream ifs("/Users/jiri/School/thesis/assembler/test", std::ios_base::binary | std::ios_base::ate);
    auto size = ifs.tellg();
    std::vector<u8> program(size);
    ifs.seekg(0, std::ios_base::beg);
    ifs.read(reinterpret_cast<char *>(program.data()), size);

    this->mcu.load_program(program);

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

    /* Timer */
    this->timer = new QTimer;
    this->timer->setInterval(1000);
    this->timer->start();

    connect(this->timer, &QTimer::timeout, [this]() {
        this->mcu.step();
        this->instructionView->update();
        this->registerView->update();
        this->memoryView->update();
    });
}

MainWindow::~MainWindow()
{ }
