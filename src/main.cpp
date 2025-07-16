#include <environment/graph.hpp>
#include <environment/graph_guidance.hpp>
#include <environment/heuristic_matrix.hpp>
#include <environment/info.hpp>
#include <environment/map.hpp>
#include <environment/robot.hpp>
#include <environment/task.hpp>
#include <environment/test_system.hpp>

#include <planner/epibt/operations.hpp>
#include <planner/epibt/operations_map.hpp>

#include <utils/config.hpp>
#include <utils/tools.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numeric>

int main(int argc, char *argv[]) {
    RuntimeConfig config;

    std::cout << "Threads_num: " << THREADS_NUM_DEFAULT << std::endl;

    if (argc >= 2) {
        std::string config_file = argv[1];
        std::cout << "Loading config file: " << config_file << std::endl;
        config = load_config(config_file);
    } else {
        std::cout << "No config file specified, using default values" << std::endl;
    }
    apply_runtime_config(config);

    std::ifstream(config.map_file) >> get_map();
    TaskPool task_pool;
    std::ifstream(config.tasks_file) >> task_pool;

    // environment
    get_gg() = GraphGuidance(get_map().get_rows(), get_map().get_cols());
    get_graph() = Graph(get_map(), get_gg());
    get_hm() = HeuristicMatrix(get_graph());

    // epibt
    init_operations();
    get_omap() = OperationsMap(get_graph(), get_operations());

    std::vector<uint32_t> agents_nums;
    if (get_map_type() == MapType::SORTATION || get_map_type() == MapType::WAREHOUSE || get_map_type() == MapType::CITY) {
        agents_nums = {
                1000,
                2000,
                3000,
                4000,
                5000,
                6000,
                7000,
                8000,
                9000,
                10000,
        };
    } else if (get_map_type() == MapType::GAME) {
        agents_nums = {
                500,
                1000,
                1500,
                2000,
                2500,
                3000,
                3500,
                4000,
                4500,
                5000,
        };
    } else if (get_map_type() == MapType::RANDOM) {
        agents_nums = {
                100,
                200,
                300,
                400,
                500,
                600,
                700,
                800,
        };
    }

    std::filesystem::create_directories(config.output_dir);

    std::mutex mutex;

    std::vector<bool> visited(agents_nums.size());

    launch_threads(THREADS_NUM, [&](uint32_t thr) {
        for (int test = agents_nums.size() - 1; test >= 0; test--) {
            {
                std::unique_lock locker(mutex);
                if (visited[test]) {
                    continue;
                }
                visited[test] = true;
            }

            Timer timer;

            Robots robots;
            std::ifstream(config.agents_file + "/agents_" + std::to_string(agents_nums[test]) + ".csv") >> robots;

            TestSystem test_system(robots, task_pool);

            std::string test_dir = config.output_dir + std::to_string(test) + "/";
            std::filesystem::create_directories(test_dir);

            Answer answer = test_system.simulate(config.steps_num);

            answer.write_log(std::ofstream(test_dir + "log.csv"));

            for (uint32_t action = 0; action <= ACTIONS_NUM; action++) {
                std::string filename = test_dir + "heatmap_";
                if (action < ACTIONS_NUM) {
                    filename += action_to_char(static_cast<ActionType>(action));
                } else {
                    filename += "all";
                }
                filename += ".csv";
                answer.write_heatmap(std::ofstream(filename), action);
            }

            {
                std::ofstream output(test_dir + "metrics.csv");
                output << "metric,value\n";
                output << "map type," << map_type_to_string(config.map_type) << '\n';
                output << "scheduler type," << scheduler_type_to_string(config.scheduler_type) << '\n';
                output << "planner type," << planner_type_to_string(config.planner_type);
                if (config.planner_type == PlannerType::EPIBT || config.planner_type == PlannerType::EPIBT_LNS || config.planner_type == PlannerType::PEPIBT_LNS) {
                    output << "(" << EPIBT_DEPTH_VALUE << ")\n";
                }
                output << "agents num," << robots.size() << '\n';
                output << "steps num," << config.steps_num << '\n';
                output << "finished tasks," << answer.finished_tasks.size() << '\n';
                output << "throughput," << static_cast<double>(answer.finished_tasks.size()) / config.steps_num << '\n';
                for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
                    output << action_to_char(static_cast<ActionType>(action)) << "," << static_cast<double>(answer.actions_num.back()[action]) * 100 / config.steps_num / robots.size() << "\n";
                }
                output << "avg step time (ms)," << std::accumulate(answer.step_time.begin(), answer.step_time.end(), 0.0) * 1000 / config.steps_num << '\n';
                output << "avg scheduler time (ms)," << std::accumulate(answer.scheduler_time.begin(), answer.scheduler_time.end(), 0.0) * 1000 / config.steps_num << '\n';
                output << "avg planner time (ms)," << std::accumulate(answer.planner_time.begin(), answer.planner_time.end(), 0.0) * 1000 / config.steps_num << '\n';

                {
                    std::unique_lock locker(mutex);
                    std::cout << "Done test " << test << ' ' << timer << '\n';
                }
            }
        }
    });

    std::cout << "\nDone" << std::endl;
}
