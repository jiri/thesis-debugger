#-------------------------------------------------
#
# Project created by QtCreator 2018-04-28T20:36:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = debugger
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        src/instructionview.cpp \
        src/disassemblymodel.cpp \
        src/registerview.cpp \
        src/registermodel.cpp \
        src/memoryview.cpp \
        src/memorymodel.cpp \
        src/mcustate.cpp \
        src/flagview.cpp

HEADERS += \
        src/mainwindow.h \
        src/instructionview.h \
        src/disassemblymodel.h \
        src/registerview.h \
        src/registermodel.h \
        src/memoryview.h \
        src/memorymodel.h \
        src/mcustate.h \
        src/flagview.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../emulator/cmake-build-debug/release/ -lemulator
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../emulator/cmake-build-debug/debug/ -lemulator
else:unix: LIBS += -L$$PWD/../emulator/cmake-build-debug/ -lemulator

INCLUDEPATH += $$PWD/../emulator/src
DEPENDPATH += $$PWD/../emulator/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../emulator/cmake-build-debug/release/libemulator.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../emulator/cmake-build-debug/debug/libemulator.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../emulator/cmake-build-debug/release/emulator.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../emulator/cmake-build-debug/debug/emulator.lib
else:unix: PRE_TARGETDEPS += $$PWD/../emulator/cmake-build-debug/libemulator.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/lib/release/ -lfmt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/lib/debug/ -lfmt
else:unix: LIBS += -L$$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/lib/ -lfmt

INCLUDEPATH += $$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/include
DEPENDPATH += $$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/lib/release/libfmt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/lib/debug/libfmt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/lib/release/fmt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/lib/debug/fmt.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/Cellar/fmt/4.1.0/lib/libfmt.a
