#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <planner/wppl/LNS/ConstraintTable.h>
#include <planner/wppl/LNS/Instance.h>
#include <planner/wppl/LNS/Parallel/DataStructure.h>
#include <planner/wppl/LNS/Parallel/TimeSpaceAStarState.h>
#include <planner/wppl/util/HeuristicTable.h>
#include <planner/wppl/util/TimeLimiter.h>

#include <boost/unordered_set.hpp>

#include <utility>

namespace LNS {

    namespace Parallel {

        class TimeSpaceAStarPlanner {
        public:
            Instance &instance;
            std::shared_ptr<HeuristicTable> HT;
            std::shared_ptr<vector<float>> weights;
            int execution_window;

            boost::heap::pairing_heap<TimeSpaceAStarState *, boost::heap::compare<TimeSpaceAStarState::Compare>> open_list;
            boost::unordered_set<TimeSpaceAStarState *, TimeSpaceAStarState::Hash, TimeSpaceAStarState::Equal> all_states;

            std::vector<TimeSpaceAStarState *> successors;

            int n_expanded;
            int n_generated;

            // results
            Path path;

            static const int n_dirs = 5;   // east, south, west, north, stay
            static const int n_orients = 4;// east, south, west, north

            TimeSpaceAStarPlanner(Instance &instance, std::shared_ptr<HeuristicTable> HT, std::shared_ptr<vector<float>> weights, int execution_window);
            void findPath(int start_pos, int start_orient, int goal_pos, ConstraintTable &constraint_table, const TimeLimiter &time_limiter);
            void clear();
            void buildPath(TimeSpaceAStarState *curr, int goal_pos);
            void getSuccessors(TimeSpaceAStarState *state, int goal_pos, ConstraintTable &constraint_table);
        };

    }// namespace Parallel

}// namespace LNS
#endif