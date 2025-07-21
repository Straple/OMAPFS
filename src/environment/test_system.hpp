#pragma once

#include <environment/answer.hpp>
#include <environment/environment.hpp>
#include <environment/robot.hpp>
#include <environment/task.hpp>
#include <planner/causal_pibt/planner.hpp>
#include <planner/wppl/wppl.hpp>
#include <scheduler/greedy/greedy_scheduler.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class TestSystem {

    uint32_t timestep = 0;

    Robots robots;

    TaskPool task_pool;

    std::vector<uint32_t> finished_tasks;

    std::vector<uint32_t> epibt_prev_operations;

    Environment env;

    std::unique_ptr<GreedyScheduler> greedy_scheduler;

#ifdef ENABLE_ROTATE_MODEL
    std::unique_ptr<CausalPIBT> causal_pibt_planner;

    std::unique_ptr<WPPL> wppl_planner;
#endif

    void gen_random_agents();

    void update(Answer &answer);

    std::vector<uint32_t> get_schedule();

    std::vector<ActionType> get_actions();

public:
    TestSystem(Robots robots, TaskPool task_pool);

    Answer simulate(uint32_t steps_num);
};
