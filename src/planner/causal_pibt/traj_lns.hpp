#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/heap.hpp>
#include <planner/causal_pibt/heuristics.hpp>
#include <planner/causal_pibt/memory.hpp>
#include <planner/causal_pibt/search_node.hpp>
#include <planner/causal_pibt/types.hpp>
#include <utils/time.hpp>

#include <iostream>
#include <random>
#include <set>

namespace CausalPlanner {
    struct FW_Metric {
        int id = 0;
        int deviation = 0;
        int last_replan_t = 0;
        int rand = 0;

        FW_Metric(int i, int d, int l);
        FW_Metric() = default;
    };

    class TrajLNS {
    public:
        Environment *env;
        std::vector<int> tasks;

        TimePoint start_time;
        int t_ms = 0;

        std::vector<Traj> trajs;

        std::vector<std::pair<int, int>> deviation_agents;

        std::vector<Int4> flow;
        std::vector<HeuristicTable> &heuristics;
        std::vector<Dist2Path> traj_dists;
        std::vector<s_node> goal_nodes;// store the goal node of single agent search for each agent. contains all cost information.

        std::vector<FW_Metric> fw_metrics;
        Neighbors &neighbors;

        int traj_inited = 0;
        int dist2path_inited = 0;
        int soc = 0;

        MemoryPool mem;

        std::mt19937 rnd;

        void init_mem();

        TrajLNS(Environment *env, std::vector<HeuristicTable> &heuristics, Neighbors &neighbors);

        TrajLNS();
    };
}// namespace CausalPlanner

#endif
