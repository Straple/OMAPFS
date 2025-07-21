#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <environment/action_model.hpp>
#include <planner/causal_pibt/traj_lns.hpp>
#include <planner/causal_pibt/types.hpp>
#include <utils/time.hpp>

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
    std::vector<CausalPlanner::DCR> decided;
    std::vector<bool> checked;
    std::vector<bool> require_guide_path;
    std::vector<int> dummy_goals;
    CausalPlanner::TrajLNS trajLNS;

public:
    void initialize(Environment *env);

    void plan(TimePoint end_time, std::vector<ActionType> &actions, Environment *env);
};

#endif