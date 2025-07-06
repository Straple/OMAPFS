#include <environment/test_system.hpp>

#include <environment/graph.hpp>
#include <environment/graph_guidance.hpp>
#include <environment/heuristic_matrix.hpp>
#include <environment/info.hpp>
#include <environment/map.hpp>
#include <environment/robot.hpp>
#include <environment/task.hpp>

#include <basic/randomizer.hpp>

#include <scheduler/greedy/greedy_scheduler.hpp>

#include <planner/epibt/epibt.hpp>
#include <planner/epibt/epibt_lns.hpp>
#include <planner/epibt/operations.hpp>
#include <planner/epibt/operations_map.hpp>
#include <planner/epibt/pepibt_lns.hpp>

#include <fstream>
#include <iomanip>
#include <map>
#include <set>

void TestSystem::gen_random_agents() {
    Randomizer rnd;
    std::set<uint32_t> agents;
    uint32_t agents_num = 800;
    while (agents.size() < agents_num) {
        uint32_t pos = rnd.get(1, get_map().get_size() - 1);
        if (get_map().is_free(pos) && !agents.contains(pos)) {
            agents.insert(pos);
        }
    }
    {
        std::ofstream output("agents_" + std::to_string(agents_num) + ".csv");
        output << "agent id,row,col\n";
        uint32_t id = 0;
        for (uint32_t pos: agents) {
#ifdef ENABLE_ROTATE_MODEL
            Position p(pos, 0);
#else
            Position p(pos);
#endif
            output << id << ',' << p.get_row() << ',' << p.get_col() << '\n';
            id++;
        }
    }
    std::exit(0);
}

void TestSystem::update() {
    // update tasks
    for (uint32_t r = 0; r < get_robots().size(); r++) {
        auto &robot = get_robots()[r];
        if (robot.task_id != -1) {
            auto &task = get_task_pool().at(robot.task_id);
            if (robot.pos.get_pos() == task.targets[0]) {
                task.is_taken = true;
                task.targets.erase(task.targets.begin());
                if (task.targets.empty()) {
                    robot.task_id = -1;
                    robot.target = 0;
                    robot.priority = -1;
                    finished_tasks.push_back(task.task_id);
                    get_task_pool().erase(task.task_id);
                }
            }
        }
    }

    // update metrics
    for (uint32_t r = 0; r < get_robots().size(); r++) {
        auto &robot = get_robots()[r];
        if (robot.task_id != -1) {
            auto &task = get_task_pool().at(robot.task_id);
            robot.target = task.targets[0];
            robot.priority = 0;

            uint32_t d = get_hm().get(robot.node, robot.target);
            for (int i = 0; i + 1 < task.targets.size(); i++) {
                uint32_t source = task.targets[i];
                uint32_t target = task.targets[i + 1];
#ifdef ENABLE_ROTATE_MODEL
                Position pos(source, 0);
#else
                Position pos(source);
#endif
                d += get_hm().get(get_graph().get_node(pos), target);
            }
            robot.priority = d;
        }
    }
}

TestSystem::TestSystem(const std::string &map_filename, const std::string &task_filename, const std::string &robot_filename) {
    std::ifstream(map_filename) >> get_map();
    std::ifstream(task_filename) >> get_task_pool();

    // environment
    get_gg() = GraphGuidance(get_map().get_rows(), get_map().get_cols());
    get_graph() = Graph(get_map(), get_gg());
    get_hm() = HeuristicMatrix(get_graph());

    // gen_random_agents();

    std::ifstream(robot_filename) >> get_robots();

    // epibt
    init_operations();
    get_omap() = OperationsMap(get_graph(), get_operations());
}

void TestSystem::simulate(uint32_t steps_num) {
    GreedyScheduler scheduler;

    Timer total_timer;

    std::ofstream logger("log.csv");
    logger << "timestep,finished tasks,";
    for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
        logger << action_to_char(static_cast<ActionType>(action)) << ',';
    }
    logger << "time(ms),scheduler time(ms),planner time(ms)\n";
    uint64_t total_finished_tasks = 0;
    std::array<uint32_t, ACTIONS_NUM> total_actions_num{};
    uint64_t total_scheduler_time = 0;
    uint64_t total_planner_time = 0;

    for (uint32_t step = 0; step < steps_num; step++) {
        Timer step_timer;

        get_curr_timestep() = step;

        //std::cout << "\nTimestep: " << step << std::endl;

        update();

        // update task pool
        while (get_task_pool().size() < get_robots().size() * 1.5) {
            get_task_pool().gen_next_task();
        }

        // call scheduler
        std::vector<uint32_t> schedule;
        uint32_t scheduler_time = 0;
        {
            Timer timer;
            scheduler.update();
            scheduler.rebuild_dp(get_now() + Milliseconds(100));
            scheduler.solve(get_now() + Milliseconds(100));
            schedule = scheduler.get_schedule();
            scheduler_time = timer.get_ms();
            total_scheduler_time += timer.get_ns();
        }

        // TODO: validate new schedule

        // set new schedule
        for (uint32_t r = 0; r < get_robots().size(); r++) {
            uint32_t old_t = get_robots()[r].task_id;
            if (old_t != -1) {
                get_task_pool().at(old_t).agent_assigned = -1;
            }
            get_robots()[r].task_id = schedule[r];
            get_task_pool().at(schedule[r]).agent_assigned = r;
        }

        update();

        // call planner
        std::vector<ActionType> actions;
        uint32_t planner_time = 0;
        {
            Timer timer;
            TimePoint end_time = get_now() + Milliseconds(100);
            if (get_planner_type() == PlannerType::EPIBT) {
                EPIBT epibt(end_time);
                epibt.solve();
                actions = epibt.get_actions();
            } else if (get_planner_type() == PlannerType::EPIBT_LNS) {
                EPIBT_LNS pibt(end_time);
                pibt.solve(42);
                actions = pibt.get_actions();
            } else if (get_planner_type() == PlannerType::PEPIBT_LNS) {
                PEPIBT_LNS pibt(end_time);
                pibt.solve(42);
                actions = pibt.get_actions();
            } else {
                FAILED_ASSERT("unexpected planner type");
            }
            planner_time = timer.get_ms();
            total_planner_time += timer.get_ns();
        }

        // validate actions
        {
            std::map<uint32_t, uint32_t> pos_usage, edge_usage;
            for (uint32_t r = 0; r < get_robots().size(); r++) {
                auto &robot = get_robots()[r];
                Position to = robot.pos.simulate(actions[r]);
                ASSERT(!pos_usage.contains(to.get_pos()), "vertex collision");
                pos_usage[to.get_pos()] = r;
                ASSERT(!edge_usage.contains(get_graph().get_to_edge(robot.node, static_cast<uint32_t>(actions[r]))), "edge collision");
            }
        }

        std::array<uint32_t, ACTIONS_NUM> actions_num{};
        for (uint32_t r = 0; r < get_robots().size(); r++) {
            auto &robot = get_robots()[r];
            robot.pos = robot.pos.simulate(actions[r]);
            robot.node = get_graph().get_node(robot.pos);
            actions_num[static_cast<uint32_t>(actions[r])]++;
            total_actions_num[static_cast<uint32_t>(actions[r])]++;
        }

        update();

        logger << step << ',' << finished_tasks.size() << ',';
        for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
            logger << actions_num[action] << ',';
        }
        logger << step_timer.get_ms() << ',' << scheduler_time << ',' << planner_time << std::endl;
        total_finished_tasks += finished_tasks.size();
        finished_tasks.clear();
    }
    logger << std::fixed << std::setprecision(1);
    logger << "total," << total_finished_tasks << ',';
    for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
        logger << total_actions_num[action] * 100.0 / steps_num / get_robots().size() << "%,";
    }
    logger << total_timer.get_ms() << ',' << total_scheduler_time / 1'000'000 << ',' << total_planner_time / 1'000'000 << std::endl;
}
