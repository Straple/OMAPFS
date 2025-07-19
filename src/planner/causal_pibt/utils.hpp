#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/types.hpp>

namespace DefaultPlanner {
    int get_d(int diff, const SharedEnvironment *env);

    bool validateMove(int loc, int loc2, const SharedEnvironment *env);

    int manhattanDistance(int loc, int loc2, const SharedEnvironment *env);

    void getNeighbors(const SharedEnvironment *env, std::vector<std::pair<int, int>> &neighbors, int location,
                      int direction);

    void getNeighbors_nowait(const SharedEnvironment *env, std::vector<std::pair<int, int>> &neighbors, int location,
                             int direction);

    void getNeighborLocs(const Neighbors *ns, std::vector<int> &neighbors, int location);

    void getNeighborLocs(const Neighbors *ns, int neighbors[], int location);
}// namespace DefaultPlanner

#endif