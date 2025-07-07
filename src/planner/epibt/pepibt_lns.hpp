#pragma once

#include <basic/time.hpp>
#include <environment/robot.hpp>
#include <environment/action_model.hpp>

// Parallel Enhanced Priority Inheritance with BackTracking + Large Neighborhood Search
class PEPIBT_LNS {

    TimePoint end_time;

    std::vector<ActionType> best_actions;

    std::vector<uint32_t> best_desires;

public:
    PEPIBT_LNS(TimePoint end_time, const std::vector<uint32_t> &operations);

    void solve(uint64_t seed);

    [[nodiscard]] std::vector<uint32_t> get_desires() const;

    [[nodiscard]] std::vector<ActionType> get_actions() const;
};
