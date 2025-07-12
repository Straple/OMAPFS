#pragma once

#include <environment/info.hpp>

#include <cstdint>
#include <string>

// Runtime configurable settings (only parameters actually used in code)
struct RuntimeConfig {
    MapType map_type = MapType::RANDOM;
    PlannerType planner_type = PlannerType::EPIBT;
    SchedulerType scheduler_type = SchedulerType::CONST;

    // TestSystem file paths
    std::string map_file = "tests/random/map.txt";
    std::string tasks_file = "tests/random/tasks.csv";
    std::string agents_file = "tests/random/agents_400.csv";
    std::string output_dir = "solutions";
    uint32_t steps_num = 1000;
};

// Compile-time settings (will be defined by CMake)
#ifndef EPIBT_DEPTH
#define EPIBT_DEPTH 3
#endif

#ifndef THREADS_NUM_DEFAULT
#define THREADS_NUM_DEFAULT 8
#endif

RuntimeConfig load_config(const std::string &config_file = "config.json");

void save_config(const RuntimeConfig &config, const std::string &config_file = "config.json");

void apply_runtime_config(const RuntimeConfig &config);
