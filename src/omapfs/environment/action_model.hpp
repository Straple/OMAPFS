#pragma once

#include <settings.hpp>

enum class ActionType {
#ifdef ENABLE_ROTATE_MODEL
    FORWARD,
    ROTATE,
    COUNTER_ROTATE,
#else
    EAST, // >
    SOUTH, // v
    WEST, // <
    NORTH, // ^
#endif

    WAIT,
};

#ifdef ENABLE_ROTATE_MODEL
enum class DirectionType {
    EAST, // >
    SOUTH, // v
    WEST, // <
    NORTH, // ^
};
#endif
