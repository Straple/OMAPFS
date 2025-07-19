#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include "TrajLNS.h"
#include "Types.h"

#include <random>

#include <environment/action_model.hpp>

namespace DefaultPlanner {
    void initialize(int preprocess_time_limit, SharedEnvironment *env);

    void plan(int time_limit, std::vector<ActionType> &actions, SharedEnvironment *env);
}// namespace DefaultPlanner

#endif