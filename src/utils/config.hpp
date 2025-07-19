#pragma once

#include <environment/info.hpp>

#include <cstdint>
#include <string>

// Runtime configurable settings (only parameters actually used in code)
struct RuntimeConfig {
    MapType map_type = MapType::RANDOM;
    PlannerType planner_type = PlannerType::EPIBT;
    SchedulerType scheduler_type = SchedulerType::CONST;
    GraphGuidanceType graph_guidance_type = GraphGuidanceType::DISABLE;

    // TestSystem file paths
    std::string map_file = "tests/random/map.txt";
    std::string tasks_path = "tests/random/tasks.csv";
    std::string agents_path = "tests/random/agents_0.csv";
    std::string output_path = "solutions";
    uint32_t steps_num = 1000;
};

RuntimeConfig load_config(const std::string &config_file = "config.json");

void save_config(const RuntimeConfig &config, const std::string &config_file = "config.json");

void apply_runtime_config(const RuntimeConfig &config);
