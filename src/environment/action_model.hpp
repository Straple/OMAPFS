#pragma once

#include <settings.hpp>

#include <array>
#include <cstdint>

enum class ActionType : uint8_t {
#ifdef ENABLE_ROTATE_MODEL
    FORWARD,
    ROTATE,
    COUNTER_ROTATE,
#else
    EAST, // >
    SOUTH,// v
    WEST, // <
    NORTH,// ^
#endif
    WAIT,
};

constexpr uint32_t ACTIONS_NUM = static_cast<uint32_t>(ActionType::WAIT) + 1;

char action_to_char(ActionType action);

ActionType char_to_action(char symbol);

#ifdef ENABLE_ROTATE_MODEL
enum class DirectionType : uint8_t {
    EAST, // >
    SOUTH,// v
    WEST, // <
    NORTH,// ^
};

constexpr uint32_t DIRECTIONS_NUM = static_cast<uint32_t>(DirectionType::NORTH) + 1;
#endif
