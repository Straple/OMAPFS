#pragma once

#include <cstdint>
#include <string>
#include <vector>

class TestSystem {

    std::vector<uint32_t> finished_tasks;

    void gen_random_agents();

    void update();

public:
    TestSystem() = default;

    TestSystem(const std::string &map_filename, const std::string &task_filename, const std::string &robot_filename);

    void simulate(uint32_t steps_num);
};
