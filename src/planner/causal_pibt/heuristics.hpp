#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <environment/environment.hpp>
#include <planner/causal_pibt/search_node.hpp>
#include <planner/causal_pibt/traj_lns.hpp>
#include <planner/causal_pibt/types.hpp>
#include <planner/causal_pibt/utils.hpp>

namespace CausalPlanner {

    void init_heuristics(Environment *env);

    void init_neighbor(Environment *env);

    void init_heuristic(HeuristicTable &ht, Environment *env, int goal_location);

    int get_heuristic(HeuristicTable &ht, Environment *env, int source, Neighbors *ns);

    int get_h(Environment *env, int source, int target);


    void init_dist_2_path(Dist2Path &dp, Environment *env, Traj &path);

    std::pair<int, int> get_source_2_path(Dist2Path &dp, Environment *env, int source, Neighbors *ns);

    int get_dist_2_path(Dist2Path &dp, Environment *env, int source, Neighbors *ns);

}// namespace CausalPlanner
#endif