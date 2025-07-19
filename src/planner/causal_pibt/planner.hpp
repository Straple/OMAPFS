#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <environment/action_model.hpp>
#include <planner/causal_pibt/traj_lns.hpp>
#include <planner/causal_pibt/types.hpp>
#include <utils/time.hpp>

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

    public:
        void initialize(SharedEnvironment *env);

        void plan(TimePoint end_time, std::vector<ActionType> &actions, SharedEnvironment *env);
    };
}// namespace DefaultPlanner

#endif