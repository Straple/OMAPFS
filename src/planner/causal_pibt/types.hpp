#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <environment/environment.hpp>

#include <cassert>
#include <climits>
#include <deque>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <vector>

#define MAX_TIMESTEP INT_MAX / 2

namespace CausalPlanner {
    enum DONE {
        NOT_DONE,
        DONE
    };

    struct Int4 {
        int d[4]{};
    };

    struct Int2 {
        int d[4]{};
    };

    struct DCR {
        int loc = 0;
        int state = 0;
    };

    typedef std::vector<int> Traj;

    struct PIBT_C {
        int location = 0;
        int heuristic = 0;
        int orientation = 0;// 0:east, 1:south, 2:west, 3:north
        int tie_breaker = 0;

        //constructor
        PIBT_C(int location, int heuristic, int orientation, int tie_breaker);
    };

    struct HNode {
        int label = 0;
        int location = 0;
        int direction = 0;
        int value = 0;
        int other = 0;

        unsigned int priority = 0;

        unsigned int get_priority() const;

        void set_priority(unsigned int p);

        HNode() = default;

        HNode(int location, int direction, int value);

        // the following is used to compare nodes in the OPEN list
        struct compare_node {
            bool operator()(const HNode &n1, const HNode &n2) const;
        };// used by OPEN (open) to compare nodes (top of the open has min f-val, and then highest g-val)
    };

    struct HeuristicTable {
        std::vector<int> htable;
        std::deque<HNode> open;

        bool empty();
    };


    struct d2p {
        int label = 0;
        int id = -1;
        int cost = -1;
        int togo = -1;

        d2p(int label, int id, int cost, int togo);
    };

    struct Dist2Path {
        int label = 0;
        std::vector<d2p> dist2path;
        std::deque<d2p> open;

        bool empty();
    };

    typedef std::vector<std::vector<int>> Neighbors;

}// namespace CausalPlanner

#endif