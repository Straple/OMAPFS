#include <environment/info.hpp>

#include <array>
#include <map>

// ==================================

std::string map_type_to_string(MapType type) {
    const static std::array<std::string, static_cast<uint32_t>(MapType::NUM)> map = {
            "BERLIN_1_256",
            "BERLIN_1_256_SMALL",

            "PARIS_1_256",
            "PARIS_1_256_SMALL",

            "RANDOM_256_10",
            "RANDOM_256_10_SMALL",

            "RANDOM_256_20",
            "RANDOM_256_20_SMALL",

            "SORTATION_LARGE",
            "SORTATION_SMALL",

            "WAREHOUSE_LARGE",
            "WAREHOUSE_SMALL",
    };
    return map.at(static_cast<uint32_t>(type));
}

MapType string_to_map_type(const std::string &str) {
    const static std::map<std::string, MapType> map = {
            {"BERLIN_1_256",MapType::BERLIN_1_256},
            {"BERLIN_1_256_SMALL",MapType::BERLIN_1_256_SMALL},

            {"PARIS_1_256",MapType::PARIS_1_256},
            {"PARIS_1_256_SMALL",MapType::PARIS_1_256_SMALL},

            {"RANDOM_256_10",MapType::RANDOM_256_10},
            {"RANDOM_256_10_SMALL",MapType::RANDOM_256_10_SMALL},

            {"RANDOM_256_20",MapType::RANDOM_256_20},
            {"RANDOM_256_20_SMALL",MapType::RANDOM_256_20_SMALL},

            {"SORTATION_LARGE",MapType::SORTATION_LARGE},
            {"SORTATION_SMALL",MapType::SORTATION_SMALL},

            {"WAREHOUSE_LARGE",MapType::WAREHOUSE_LARGE},
            {"WAREHOUSE_SMALL",MapType::WAREHOUSE_SMALL},
    };
    return map.at(str);
}

std::string planner_type_to_string(PlannerType type) {
    const static std::array<std::string, static_cast<uint32_t>(PlannerType::NUM)> map = {
            "PIBT",
            "PIBT_TF",
            "EPIBT",
            "EPIBT_LNS",
            "PEPIBT_LNS",
            "WPPL",
    };
    return map.at(static_cast<uint32_t>(type));
}

PlannerType string_to_planner_type(const std::string &str) {
    const static std::map<std::string, PlannerType> map = {
            {"PIBT", PlannerType::PIBT},
            {"PIBT_TF", PlannerType::PIBT_TF},
            {"EPIBT", PlannerType::EPIBT},
            {"EPIBT_LNS", PlannerType::EPIBT_LNS},
            {"PEPIBT_LNS", PlannerType::PEPIBT_LNS},
            {"WPPL", PlannerType::WPPL},
    };
    return map.at(str);
}

std::string scheduler_type_to_string(SchedulerType type) {
    const static std::array<std::string, static_cast<uint32_t>(SchedulerType::NUM)> map = {
            "CONST",
            "GREEDY",
    };
    return map.at(static_cast<uint32_t>(type));
}

SchedulerType string_to_scheduler_type(const std::string &str) {
    const static std::map<std::string, SchedulerType> map = {
            {"CONST", SchedulerType::CONST},
            {"GREEDY", SchedulerType::GREEDY},
    };
    return map.at(str);
}

// ==================================

MapType &get_map_type() {
    static MapType type = MapType::NUM;
    return type;
}

PlannerType &get_planner_type() {
    static PlannerType type = PlannerType::EPIBT;
    return type;
}

SchedulerType &get_scheduler_type() {
    static SchedulerType type = SchedulerType::CONST;
    return type;
}
