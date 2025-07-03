project(omapfs)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)

include_directories("src")

file(GLOB SOURCES
        src/omapfs/basic/*.cpp
        src/omapfs/environment/*.cpp

        src/omapfs/planner/*.cpp
        src/omapfs/planner/epibt/*.cpp

        src/omapfs/scheduler/greedy/*.cpp
        )

add_executable(main src/main.cpp ${SOURCES})
