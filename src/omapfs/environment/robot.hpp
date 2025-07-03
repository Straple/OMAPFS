#pragma once

#include <omapfs/environment/position.hpp>

#include <istream>
#include <vector>

struct Robot {
    uint32_t node = 0;
    Position pos;
    uint32_t task_id = -1;
};

class Robots {
    std::vector<Robot> robots;

public:
    Robots() = default;

    explicit Robots(uint32_t robots_num);

    Robot &operator[](uint32_t index);

    const Robot &operator[](uint32_t index) const;

    [[nodiscard]] uint32_t size() const;

    friend std::istream &operator>>(std::istream &input, Robots &robots);
};

Robots &get_robots();
