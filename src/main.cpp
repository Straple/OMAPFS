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

#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    RuntimeConfig config;

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

    Robots robots;
    std::ifstream(config.agents_file) >> robots;

    // epibt
    init_operations();
    get_omap() = OperationsMap(get_graph(), get_operations());

    TestSystem test_system(robots, task_pool);

    Answer answer = test_system.simulate(config.steps_num);
    for (uint32_t action = 0; action <= ACTIONS_NUM; action++) {
        std::string filename = "heatmap_";
        if (action < ACTIONS_NUM) {
            filename += action_to_char(static_cast<ActionType>(action));
        } else {
            filename += "all";
        }
        filename += ".csv";
        std::ofstream output(filename);
        answer.write_heatmap(output, action);
    }
}
