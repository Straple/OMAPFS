#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/environment.hpp>
#include <planner/causal_pibt/search_node.hpp>
#include <planner/causal_pibt/traj_lns.hpp>
#include <planner/causal_pibt/types.hpp>
#include <planner/causal_pibt/utils.hpp>

namespace DefaultPlanner {

    void init_heuristics(SharedEnvironment *env);

    void init_neighbor(SharedEnvironment *env);

    void init_heuristic(HeuristicTable &ht, SharedEnvironment *env, int goal_location);

    int get_heuristic(HeuristicTable &ht, SharedEnvironment *env, int source, Neighbors *ns);

    int get_h(SharedEnvironment *env, int source, int target);


    void init_dist_2_path(Dist2Path &dp, SharedEnvironment *env, Traj &path);

    std::pair<int, int> get_source_2_path(Dist2Path &dp, SharedEnvironment *env, int source, Neighbors *ns);

    int get_dist_2_path(Dist2Path &dp, SharedEnvironment *env, int source, Neighbors *ns);

}// namespace DefaultPlanner
#endif