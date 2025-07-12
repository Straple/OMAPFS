#pragma once

#include <cstdint>
#include <string>

enum class PlannerType : uint8_t {
    PIBT,      // pibt
    PIBT_TF,   // pibt + trafficflow
    EPIBT,     // epibt
    EPIBT_LNS, // epibt + lns
    PEPIBT_LNS,// parallel epibt + lns
    WPPL,      // windowed parallel pibt + lns

    NUM,
};

std::string planner_type_to_str(PlannerType type);

PlannerType &get_planner_type();

enum class SchedulerType : uint8_t {
    CONST,
    GREEDY,
};

std::string scheduler_type_to_str(SchedulerType type);

SchedulerType &get_scheduler_type();
