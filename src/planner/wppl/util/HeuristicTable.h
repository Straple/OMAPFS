#pragma once
//#include "common.h"
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <chrono>
#include <omp.h>
#include <planner/causal_pibt/environment.hpp>
#include <planner/wppl/util/CompetitionActionModel.h>
#include <planner/wppl/util/Dev.h>
#include <planner/wppl/util/SearchForHeuristics/SpatialSearch.h>
#include <planner/wppl/util/Timer.h>
// #include "bshoshany/BS_thread_pool.hpp"

#include <cfloat>

#define MAX_HEURISTIC FLT_MAX / 16

class HeuristicTable {
public:
    const DefaultPlanner::SharedEnvironment &env;
    CompetitionActionModelWithRotate action_model;
    // loc1, loc2
    float *main_heuristics;
    // loc1, loc2, orient1, orient2
    float *sub_heuristics;
    int *empty_locs;
    int *loc_idxs;
    int n_orientations = 4;
    size_t loc_size = 0;
    size_t state_size;
    bool consider_rotation = true;

    std::shared_ptr<std::vector<float>> map_weights;

    HeuristicTable(DefaultPlanner::SharedEnvironment *_env, const std::shared_ptr<std::vector<float>> &map_weights, bool consider_rotation = true);
    ~HeuristicTable();

    // weights is an array of [loc_size*n_orientations]
    void compute_weighted_heuristics();

    void _compute_weighted_heuristics(
            int start_loc_idx,
            float *values,
            RIVERS::SPATIAL::SpatialAStar *planner);

    void dump_main_heuristics(int start_loc, string file_path_prefix);

    // void compute_heuristics();

    // void _push(State * queue, State &s, int & e_idx);
    // State _pop(State * queue, int & s_idx);
    // bool _empty(int s_idx, int e_idx);

    // void compute_heuristics(
    //     int start_loc_idx,
    //     unsigned short * values,
    //     bool * visited,
    //     State * queue
    // );

    // TODO(hj) add check
    float get(int loc1, int loc2);
    float get(int loc1, int orient1, int loc2);
    // int get(int loc1, int orient1, int loc2, int orient2);

    // void preprocess();
    void preprocess(string suffix = "");
    void save(const string &fpath);
    void load(const string &fpath);
};