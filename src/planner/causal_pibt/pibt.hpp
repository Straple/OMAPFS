#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <environment/action_model.hpp>

#include <planner/causal_pibt/heuristics.hpp>
#include <planner/causal_pibt/traj_lns.hpp>
#include <planner/causal_pibt/types.hpp>
#include <planner/causal_pibt/utils.hpp>

#include <list>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace DefaultPlanner {

    int get_gp_h(TrajLNS &lns, int ai, int target);


    bool causalPIBT(int curr_id, int higher_id, std::vector<State> &prev_states,
                    std::vector<State> &next_states,
                    std::vector<int> &prev_decision, std::vector<int> &decision,
                    std::vector<bool> &occupied, TrajLNS &lns);


    ActionType getAction(State &prev, State &next);

    ActionType getAction(State &prev, int next_loc, SharedEnvironment *env);

    bool moveCheck(int id, std::vector<bool> &checked,
                   std::vector<DCR> &decided, std::vector<ActionType> &actions, std::vector<int> &prev_decision);
}// namespace DefaultPlanner
#endif
