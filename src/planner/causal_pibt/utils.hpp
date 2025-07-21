#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/types.hpp>

namespace CausalPlanner {
    int get_d(int diff, const Environment *env);

    bool validateMove(int loc, int loc2, const Environment *env);

    int manhattanDistance(int loc, int loc2, const Environment *env);

    void getNeighbors(const Environment *env, std::vector<std::pair<int, int>> &neighbors, int location,
                      int direction);

    void getNeighbors_nowait(const Environment *env, std::vector<std::pair<int, int>> &neighbors, int location,
                             int direction);

    void getNeighborLocs(const Neighbors *ns, std::vector<int> &neighbors, int location);

    void getNeighborLocs(const Neighbors *ns, int neighbors[], int location);
}// namespace CausalPlanner

#endif