#include <omapfs/environment/robot.hpp>

#include <omapfs/basic/assert.hpp>

Robot &Robots::operator[](uint32_t index) {
    ASSERT(index < robots.size(), "invalid index");
    return robots[index];
}

const Robot &Robots::operator[](uint32_t index) const {
    ASSERT(index < robots.size(), "invalid index");
    return robots[index];
}

uint32_t Robots::size() const {
    return robots.size();
}

Robots &get_robots() {
    static Robots robots;
    return robots;
}
