#pragma once

#include <cstdint>
#include <string>

enum class MapType : uint8_t {
    BERLIN_1_256,
    BERLIN_1_256_SMALL,

    PARIS_1_256,
    PARIS_1_256_SMALL,

    RANDOM_256_10,
    RANDOM_256_10_SMALL,

    RANDOM_256_20,
    RANDOM_256_20_SMALL,

    SORTATION_LARGE,
    SORTATION_SMALL,

    WAREHOUSE_LARGE,
    WAREHOUSE_SMALL,

    NUM
};

enum class PlannerType : uint8_t {
    PIBT,      // pibt
    PIBT_TF,   // pibt + trafficflow
    EPIBT,     // epibt
    EPIBT_LNS, // epibt + lns
    PEPIBT_LNS,// parallel epibt + lns
    WPPL,      // windowed parallel pibt + lns

    NUM,
};

enum class SchedulerType : uint8_t {
    CONST,
    GREEDY,

    NUM
};

// ==================================

std::string map_type_to_string(MapType type);

MapType string_to_map_type(const std::string &str);

std::string planner_type_to_string(PlannerType type);

PlannerType string_to_planner_type(const std::string &str);

std::string scheduler_type_to_string(SchedulerType type);

SchedulerType string_to_scheduler_type(const std::string &str);

// ==================================

MapType &get_map_type();

PlannerType &get_planner_type();

SchedulerType &get_scheduler_type();
