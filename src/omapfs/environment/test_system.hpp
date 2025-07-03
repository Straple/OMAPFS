#pragma once

#include <string>

class TestSystem {

    void update();

public:
    TestSystem() = default;

    TestSystem(const std::string &map_filename, const std::string &task_filename, const std::string &robot_filename);

    void simulate(uint32_t steps_num);
};
