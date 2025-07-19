#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include "States.h"

#include <unordered_map>
#include <vector>

namespace DefaultPlanner {
    class SharedEnvironment {
    public:
        int num_of_agents = 0;
        int rows = 0;
        int cols = 0;
        std::vector<int> map;

        // goal locations for each agent
        // each task is a pair of <goal_loc, reveal_time>
        std::vector<std::vector<std::pair<int, int>>> goal_locations;

        int curr_timestep = 0;
        std::vector<State> curr_states;

        SharedEnvironment() = default;
    };
};// namespace DefaultPlanner

#endif
