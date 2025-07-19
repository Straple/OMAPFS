#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/traj_lns.hpp>

namespace DefaultPlanner {
    extern std::vector<HeuristicTable> global_heuristictable;
    extern Neighbors global_neighbors;
};// namespace DefaultPlanner

using namespace DefaultPlanner;

FW_Metric::FW_Metric(int i, int d, int l)
    : id(i), deviation(d), last_replan_t(l) {
}

void TrajLNS::init_mem() {
    mem.init(env->map.size());
}

TrajLNS::TrajLNS(SharedEnvironment *env, std::vector<HeuristicTable> &heuristics, Neighbors &neighbors)
    : env(env),
      trajs(env->num_of_agents),
      tasks(env->num_of_agents),
      flow(env->map.size(), Int4({0, 0, 0, 0})), heuristics(heuristics),
      traj_dists(env->num_of_agents), goal_nodes(env->num_of_agents),
      fw_metrics(env->num_of_agents), neighbors(neighbors){};


TrajLNS::TrajLNS() : heuristics(global_heuristictable), neighbors(global_neighbors) {
}

#endif
