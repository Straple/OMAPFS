#pragma once

#include <basic/time.hpp>
#include <environment/robot.hpp>
#include <environment/action_model.hpp>

// Parallel Enhanced Priority Inheritance with BackTracking + Large Neighborhood Search
class PEPIBT_LNS {

    TimePoint end_time;

    std::vector<ActionType> actions;

public:
    PEPIBT_LNS(TimePoint end_time);

    void solve(uint64_t seed);

    [[nodiscard]] std::vector<ActionType> get_actions() const;
};
