#include <omapfs/environment/info.hpp>

uint32_t &get_curr_timestep() {
    static uint32_t timestep = 0;
    return timestep;
}

PlannerType &get_planner_type() {
    static PlannerType type = PlannerType::EPIBT;
    return type;
}
