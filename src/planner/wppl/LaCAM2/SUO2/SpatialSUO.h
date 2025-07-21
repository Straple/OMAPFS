#pragma once
#include <omp.h>
#include <planner/causal_pibt/environment.hpp>
#include <planner/wppl/LaCAM2/SUO2/CostMap.h>
#include <planner/wppl/LaCAM2/SUO2/SpatialSearch.h>
#include <planner/wppl/util/HeuristicTable.h>

namespace SUO2 {

    namespace Spatial {

        class SUO {
        public:
            SUO(
                    const DefaultPlanner::SharedEnvironment &_env,
                    const std::vector<float> &_weights,
                    std::shared_ptr<HeuristicTable> &_HT,
                    float _vertex_collision_cost,
                    int _iterations,
                    int _max_expanded,
                    int _window,
                    float _h_weight) : env(_env),
                                       weights(_weights),
                                       HT(_HT),
                                       vertex_collision_cost(_vertex_collision_cost),
                                       iterations(_iterations),
                                       max_expanded(_max_expanded),
                                       window(_window),
                                       h_weight(_h_weight) {

                n_threads = omp_get_max_threads();

                planners = new Spatial::AStar *[n_threads];
                for (int tid = 0; tid < n_threads; ++tid) {
                    planners[tid] = new Spatial::AStar(env, weights, HT, window);
                }
            }


            ~SUO() {
                for (int tid = 0; tid < n_threads; ++tid) {
                    delete planners[tid];
                }
                delete[] planners;
            }

            int n_threads;
            Spatial::AStar **planners;

            const DefaultPlanner::SharedEnvironment &env;
            const std::vector<float> &weights;
            std::shared_ptr<HeuristicTable> HT;

            float vertex_collision_cost;
            int iterations;
            int max_expanded;
            int window;
            float h_weight;

            std::vector<std::vector<State>> paths;
            std::vector<float> path_costs;
            std::vector<int> orders;
            // std::vector<float> cost_map;
            std::vector<std::pair<int, float>> deltas;

            void init();
            void plan();
            void update_path(int agent_idx, State *goal_state);
            void reset_cost_map();
        };


    }// namespace Spatial

}// namespace SUO2