#pragma once

#include <environment/answer.hpp>
#include <environment/robot.hpp>
#include <environment/task.hpp>
#include <scheduler/greedy/greedy_scheduler.hpp>

#include <cstdint>
#include <string>
#include <vector>

class TestSystem {

    Robots robots;
    TaskPool task_pool;

    GreedyScheduler scheduler;

    std::vector<uint32_t> finished_tasks;

    void gen_random_agents();

    void update();

    std::vector<uint32_t> get_schedule();

    std::vector<ActionType> get_actions();

public:
    TestSystem() = default;

    TestSystem(Robots robots, TaskPool task_pool);

    Answer simulate(uint32_t steps_num);
};
