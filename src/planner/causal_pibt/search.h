#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include "Memory.h"
#include "Types.h"
#include "heap.h"
#include "heuristics.h"
#include "search_node.h"
#include "utils.h"

namespace DefaultPlanner {
    //a astar minimized the opposide traffic flow with existing traffic flow

    s_node astar(SharedEnvironment *env, std::vector<Int4> &flow,
                 HeuristicTable &ht, Traj &traj,
                 MemoryPool &mem, int start, int goal, Neighbors *ns);
}// namespace DefaultPlanner

#endif