#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include "TrajLNS.h"
#include "Types.h"

#include <random>

#include <environment/action_model.hpp>

namespace DefaultPlanner {
    class CausalPIBT {
        //default planner data
        std::vector<int> decision;
        std::vector<int> prev_decision;
        std::vector<double> p;
        std::vector<State> prev_states;
        std::vector<State> next_states;
        std::vector<int> ids;
        std::vector<double> p_copy;
        std::vector<bool> occupied;
        std::vector<DCR> decided;
        std::vector<bool> checked;
        std::vector<bool> require_guide_path;
        std::vector<int> dummy_goals;
        TrajLNS trajLNS;
        std::mt19937 mt1;

    public:

        void initialize(SharedEnvironment *env);

        void plan(int time_limit, std::vector<ActionType> &actions, SharedEnvironment *env);
    };
}// namespace DefaultPlanner

#endif