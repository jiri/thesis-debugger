cmake_minimum_required(VERSION 3.9)
project(debugger)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

set(SOURCE_FILES
  src/disassemblymodel.h
  src/disassemblymodel.cpp
  src/flagview.h
  src/flagview.cpp
  src/instructionview.h
  src/instructionview.cpp
  src/main.cpp
  src/mainwindow.h
  src/mainwindow.cpp
  src/mcustate.h
  src/mcustate.cpp
  src/memorymodel.h
  src/memorymodel.cpp
  src/memoryview.h
  src/memoryview.cpp
  src/registermodel.h
  src/registermodel.cpp
  src/registerview.h
  src/registerview.cpp
)

add_subdirectory(lib/emulator)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        Qt5::Widgets
        fmt
        emulator
        )
