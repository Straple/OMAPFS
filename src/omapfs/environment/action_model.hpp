#pragma once

#include <settings.hpp>

#include <cstdint>

enum class ActionType : uint8_t {
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

constexpr uint32_t ACTIONS_NUM = static_cast<uint32_t>(ActionType::WAIT) + 1;

#ifdef ENABLE_ROTATE_MODEL
enum class DirectionType : uint8_t {
    EAST, // >
    SOUTH, // v
    WEST, // <
    NORTH, // ^
};

constexpr uint32_t DIRECTIONS_NUM = static_cast<uint32_t>(DirectionType::NORTH) + 1;
#endif
