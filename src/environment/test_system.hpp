#pragma once

#include <environment/answer.hpp>
#include <environment/robot.hpp>
#include <environment/task.hpp>
#include <planner/causal_pibt/environment.hpp>
#include <planner/causal_pibt/planner.hpp>
#include <scheduler/greedy/greedy_scheduler.hpp>

#include <cstdint>
#include <string>
#include <vector>

class TestSystem {

    uint32_t timestep = 0;

    Robots robots;

    TaskPool task_pool;

    GreedyScheduler scheduler;

    std::vector<uint32_t> finished_tasks;

    std::vector<uint32_t> epibt_prev_operations;

#ifdef ENABLE_ROTATE_MODEL
    DefaultPlanner::SharedEnvironment env;
    DefaultPlanner::CausalPIBT causal_pibt;
#endif

    void gen_random_agents();

    void update(Answer &answer);

    std::vector<uint32_t> get_schedule();

    std::vector<ActionType> get_actions();

public:
    TestSystem(Robots robots, TaskPool task_pool);

    Answer simulate(uint32_t steps_num);
};
