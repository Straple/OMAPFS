#pragma once

#include <cstdint>

enum class PlannerType {
    PIBT,      // pibt
    PIBT_TF,   // pibt + trafficflow
    EPIBT,     // epibt
    EPIBT_LNS, // epibt + lns
    PEPIBT_LNS,// parallel epibt + lns
    WPPL,      // windowed parallel pibt + lns
};

PlannerType &get_planner_type();

enum class SchedulerType {
    CONST,
    GREEDY,
};

SchedulerType &get_scheduler_type();
