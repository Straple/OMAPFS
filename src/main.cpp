#include <environment/info.hpp>
#include <environment/test_system.hpp>
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

    TestSystem test_system(config.map_file, config.tasks_file, config.agents_file);
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
