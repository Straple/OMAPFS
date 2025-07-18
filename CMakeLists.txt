cmake_minimum_required(VERSION 3.16)

project(omapfs)

set(CMAKE_CXX_STANDARD 20)

# Compile-time configuration options
option(ENABLE_ROTATE_MODEL "Enable rotation model instead of directional model" OFF)
option(ENABLE_ASSERT "Enable assertions" ON)
option(ENABLE_SCHEDULER_CHANGE_TASK "Enable scheduler task changes" ON)

# Configurable compile-time values
set(EPIBT_DEPTH "2" CACHE STRING "Depth for EPIBT operations")
set(THREADS_NUM_DEFAULT "32" CACHE STRING "Default number of threads")

# Apply configuration
if (ENABLE_ROTATE_MODEL)
    add_definitions(-DENABLE_ROTATE_MODEL)
    message(STATUS "Building with ROTATION model (actions: F, R, C, w)")
else ()
    message(STATUS "Building with NON-ROTATION model (actions: E, S, W, N, w)")
endif ()

if (ENABLE_ASSERT)
    add_definitions(-DENABLE_ASSERT)
endif ()

if (ENABLE_SCHEDULER_CHANGE_TASK)
    add_definitions(-DENABLE_SCHEDULER_CHANGE_TASK)
endif ()

# Set compile-time constants
add_definitions(-DEPIBT_DEPTH=${EPIBT_DEPTH})
add_definitions(-DTHREADS_NUM_DEFAULT=${THREADS_NUM_DEFAULT})

message(STATUS "EPIBT_DEPTH set to: ${EPIBT_DEPTH}")
message(STATUS "Default threads: ${THREADS_NUM_DEFAULT}")

find_package(Threads REQUIRED)

include_directories("src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SOURCES
        src/utils/*.cpp
        src/environment/*.cpp
        src/planner/*.cpp
        src/planner/pibt/*.cpp
        src/planner/epibt/*.cpp
        src/scheduler/greedy/*.cpp
        )

add_executable(main src/main.cpp ${SOURCES})
target_link_libraries(main Threads::Threads)
