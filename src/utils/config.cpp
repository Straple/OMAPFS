#include <utils/config.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

// Helper functions for simple config file parsing
std::map<std::string, std::string> parse_simple_config(const std::string &content) {
    std::map<std::string, std::string> result;
    std::istringstream iss(content);
    std::string line;

    while (std::getline(iss, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line[0] == '/') continue;

        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Remove quotes if present
            if (value.size() >= 2 && value[0] == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }

            result[key] = value;
        }
    }
    return result;
}

RuntimeConfig load_config(const std::string &config_file) {
    RuntimeConfig config;

    std::ifstream file(config_file);
    if (!file.is_open()) {
        std::cerr << "Warning: Config file '" << config_file << "' not found, using default values" << std::endl;
        return config;
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();

    auto parsed = parse_simple_config(content);

    // Parse configuration values

    if (parsed.count("map_type")) {
        config.map_type = string_to_map_type(parsed["map_type"]);
    }
    if (parsed.count("planner_type")) {
        config.planner_type = string_to_planner_type(parsed["planner_type"]);
    }
    if (parsed.count("scheduler_type")) {
        config.scheduler_type = string_to_scheduler_type(parsed["scheduler_type"]);
    }
    if (parsed.count("map_file")) {
        config.map_file = parsed["map_file"];
    }
    if (parsed.count("tasks_file")) {
        config.tasks_file = parsed["tasks_file"];
    }
    if (parsed.count("agents_file")) {
        config.agents_file = parsed["agents_file"];
    }
    if (parsed.count("output_dir")) {
        config.output_dir = parsed["output_dir"];
        if (!config.output_dir.empty() && config.output_dir.back() != '/' && config.output_dir.back() != '\\') {
            config.output_dir += '/';
        }
    }

    if (parsed.count("steps_num")) {
        config.steps_num = std::stoi(parsed["steps_num"]);
    }

    std::cout << "Successfully loaded config from '" << config_file << "'" << std::endl;
    return config;
}

void save_config(const RuntimeConfig &config, const std::string &config_file) {
    std::ofstream file(config_file);
    if (!file.is_open()) {
        std::cerr << "Failed to save config to '" << config_file << "'" << std::endl;
        return;
    }

    file << "# OMAPFS Configuration File\n";
    file << "# Only parameters actually used in code\n\n";
    file << "planner_type = " << planner_type_to_string(config.planner_type) << "\n";
    file << "scheduler_type = " << scheduler_type_to_string(config.scheduler_type) << "\n\n";
    file << "# TestSystem file paths\n";
    file << "map_file = " << config.map_file << "\n";
    file << "tasks_file = " << config.tasks_file << "\n";
    file << "agents_file = " << config.agents_file << "\n";

    file.close();
    std::cout << "Saved config to '" << config_file << "'" << std::endl;
}

void apply_runtime_config(const RuntimeConfig &config) {
    get_map_type() = config.map_type;
    get_planner_type() = config.planner_type;
    get_scheduler_type() = config.scheduler_type;

    std::cout << "Applied runtime config:" << std::endl;
    std::cout << "  Map: " << map_type_to_string(config.map_type) << std::endl;
    std::cout << "  Planner: " << planner_type_to_string(config.planner_type) << std::endl;
    std::cout << "  Scheduler: " << scheduler_type_to_string(config.scheduler_type) << std::endl;
    std::cout << "  Map file: " << config.map_file << std::endl;
    std::cout << "  Tasks file: " << config.tasks_file << std::endl;
    std::cout << "  Agents file: " << config.agents_file << std::endl;
}