#include <omapfs/environment/test_system.hpp>

#include <omapfs/environment/graph.hpp>
#include <omapfs/environment/graph_guidance.hpp>
#include <omapfs/environment/heuristic_matrix.hpp>
#include <omapfs/environment/info.hpp>
#include <omapfs/environment/map.hpp>
#include <omapfs/environment/robot.hpp>
#include <omapfs/environment/task.hpp>

#include <omapfs/basic/randomizer.hpp>

#include <omapfs/scheduler/greedy/greedy_scheduler.hpp>

#include <omapfs/planner/epibt/operations.hpp>

#include <fstream>
#include <set>

TestSystem::TestSystem(const std::string &map_filename, const std::string &task_filename, const std::string &robot_filename) {
    std::ifstream(map_filename) >> get_map();
    std::ifstream(task_filename) >> get_task_pool();

    // environment
    get_gg() = GraphGuidance(get_map().get_rows(), get_map().get_cols());
    get_graph() = Graph(get_map(), get_gg());
    get_hm() = HeuristicMatrix(get_graph());

    // gen random agents
    /*{
        Randomizer rnd;
        std::set<uint32_t> agents;
        while (agents.size() < 100) {
            uint32_t pos = rnd.get(1, get_map().get_size() - 1);
            if (get_map().is_free(pos) && !agents.contains(pos)) {
                agents.insert(pos);
            }
        }
        std::ofstream output("agents.csv");
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
        std::exit(0);
    }*/

    std::ifstream(robot_filename) >> get_robots();

    // epibt
    init_operations();
}

void TestSystem::simulate(uint32_t steps_num) {
    GreedyScheduler scheduler;
    for (uint32_t step = 0; step < steps_num; step++) {
        get_curr_timestep() = step;

        // update task pool
        while (get_task_pool().size() < get_robots().size() * 1.5) {
            get_task_pool().gen_next_task();
        }

        // call scheduler
        scheduler.update();
        scheduler.rebuild_dp(get_now() + Milliseconds(100));
        scheduler.solve(get_now() + Milliseconds(100));
        auto schedule = scheduler.get_schedule();

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
    }
}
