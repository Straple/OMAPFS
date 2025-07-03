#pragma once

#include <omapfs/environment/position.hpp>

#include <vector>

struct Robot {
    uint32_t node = 0;
    Position pos;
    uint32_t task_id = 0;
};

class Robots {
    std::vector<Robot> robots;

public:

    Robot &operator[](uint32_t index);

    const Robot &operator[](uint32_t index) const;

    [[nodiscard]] uint32_t size() const;
};

Robots &get_robots();
