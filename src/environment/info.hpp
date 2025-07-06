#pragma once

#include <cstdint>

uint32_t &get_curr_timestep();

enum class PlannerType {
    PIBT,      // pibt
    PIBT_TF,   // pibt + trafficflow
    EPIBT,     // epibt
    EPIBT_LNS, // epibt + lns
    PEPIBT_LNS,// parallel epibt + lns
    WPPL,      // windowed parallel pibt + lns
};

PlannerType &get_planner_type();
