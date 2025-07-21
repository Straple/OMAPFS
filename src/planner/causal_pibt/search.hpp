#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/heap.hpp>
#include <planner/causal_pibt/heuristics.hpp>
#include <planner/causal_pibt/memory.hpp>
#include <planner/causal_pibt/search_node.hpp>
#include <planner/causal_pibt/types.hpp>
#include <planner/causal_pibt/utils.hpp>

namespace CausalPlanner {
    //a astar minimized the opposide traffic flow with existing traffic flow

    s_node astar(Environment *env, std::vector<Int4> &flow,
                 HeuristicTable &ht, Traj &traj,
                 MemoryPool &mem, int start, int goal, Neighbors *ns);
}// namespace CausalPlanner

#endif
