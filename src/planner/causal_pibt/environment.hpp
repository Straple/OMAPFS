#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include "States.h"

#include <unordered_map>
#include <vector>

class SharedEnvironment {
public:
    int num_of_agents;
    int rows;
    int cols;
    std::vector<int> map;

    // goal locations for each agent
    // each task is a pair of <goal_loc, reveal_time>
    std::vector<std::vector<std::pair<int, int>>> goal_locations;

    int curr_timestep = 0;
    std::vector<State> curr_states;

    SharedEnvironment() {}
};

#endif