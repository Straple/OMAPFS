#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/heuristics.hpp>
#include <planner/causal_pibt/search.hpp>
#include <planner/causal_pibt/traj_lns.hpp>
#include <planner/causal_pibt/types.hpp>

#include <random>
#include <unordered_set>

namespace CausalPlanner {

    //remove flow for each location's outgoing edge according to the traj
    void remove_traj(TrajLNS &lns, int agent);

    void add_traj(TrajLNS &lns, int agent);

    void get_deviation(TrajLNS &lns);

    void update_fw_metrics(TrajLNS &lns);

    void frank_wolfe(TrajLNS &lns, std::unordered_set<int> &updated, TimePoint timelimit);

    void update_dist_2_path(TrajLNS &lns, int i);

    //compute distance table for each traj
    void init_dist_table(TrajLNS &lns, int amount);

    //update traj and distance table for agent i
    void update_traj(TrajLNS &lns, int i);

}// namespace CausalPlanner
#endif