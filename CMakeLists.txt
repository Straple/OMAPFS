project(omapfs)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)

include_directories("src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SOURCES
        src/basic/*.cpp
        src/environment/*.cpp

        src/planner/*.cpp
        src/planner/epibt/*.cpp

        src/scheduler/greedy/*.cpp
        )

add_executable(main src/main.cpp ${SOURCES})
