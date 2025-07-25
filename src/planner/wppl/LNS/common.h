#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <planner/wppl/util/HeuristicTable.h>

#include <boost/heap/pairing_heap.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <cfloat>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip> // std::setprecision
#include <iostream>// std::cout, std::fixed
#include <limits>
#include <list>
#include <map>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

using boost::unordered_map;
using boost::unordered_set;
using boost::heap::compare;
using boost::heap::pairing_heap;
using std::clock;
using std::cout;
using std::endl;
using std::get;
using std::list;
using std::make_pair;
using std::make_shared;
using std::make_tuple;
using std::map;
using std::max;
using std::min;
using std::ofstream;
using std::pair;
using std::set;
using std::shared_ptr;
using std::string;
using std::tie;
using std::tuple;
using std::vector;
using namespace std::chrono;
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsec;

#define MAX_TIMESTEP INT_MAX
#define MAX_COST FLT_MAX / 2
#define MAX_NODES INT_MAX / 2

namespace LNS {

    struct PathEntry {
        int location = -1;
        explicit PathEntry(int loc = -1) { location = loc; }
    };

    typedef vector<PathEntry> Path;
    std::ostream &operator<<(std::ostream &os, const Path &path);
    bool isSamePath(const Path &p1, const Path &p2);

    struct IterationStats {
        int sum_of_costs;
        double runtime;
        int num_of_agents;
        string algorithm;
        int sum_of_costs_lowerbound;
        int num_of_colliding_pairs;
        IterationStats(int num_of_agents, int sum_of_costs, double runtime, const string &algorithm,
                       int sum_of_costs_lowerbound = 0, int num_of_colliding_pairs = 0) : num_of_agents(num_of_agents), sum_of_costs(sum_of_costs), runtime(runtime),
                                                                                          sum_of_costs_lowerbound(sum_of_costs_lowerbound), algorithm(algorithm),
                                                                                          num_of_colliding_pairs(num_of_colliding_pairs) {}
    };

    struct PIBTPPS_option {
        int windowSize;
        bool winPIBTSoft;
        int timestepLimit;
    };


    // Only for three-tuples of std::hash-able types for simplicity.
    // You can of course template this struct to allow other hash functions
    /*struct three_tuple_hash {
    template <class T1, class T2, class T3>
    std::size_t operator () (const std::tuple<T1, T2, T3> &p) const {
        auto h1 = std::hash<T1>{}(get<0>(p));
        auto h2 = std::hash<T2>{}(get<1>(p));
        auto h3 = std::hash<T3>{}(get<2>(p));
        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2 ^ h3;
    }
};*/


}// end namespace LNS
#endif