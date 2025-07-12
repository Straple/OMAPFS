#include <environment/info.hpp>

#include <array>

std::array<std::string, static_cast<uint32_t>(PlannerType::NUM)> planner_type_to_str_dp = {
        "pibt",
        "pibt_tf",
        "epibt",
        "epibt_lns",
        "pepibt_lns",
        "wppl",
};

std::string planner_type_to_str(PlannerType type) {
    return planner_type_to_str_dp[static_cast<uint32_t>(type)];
}

PlannerType &get_planner_type() {
    static PlannerType type = PlannerType::EPIBT;
    return type;
}

std::array<std::string, static_cast<uint32_t>(PlannerType::NUM)> scheduler_type_to_str_dp = {
        "const",
        "greedy",
};

std::string scheduler_type_to_str(SchedulerType type) {
    return scheduler_type_to_str_dp[static_cast<uint32_t>(type)];
}

SchedulerType &get_scheduler_type() {
    static SchedulerType type = SchedulerType::CONST;
    return type;
}
