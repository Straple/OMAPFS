#pragma once
#ifdef ENABLE_ROTATE_MODEL
#include <random>

#include <environment/state.hpp>
#include <planner/wppl/LNS/common.h>
#include <planner/wppl/LaCAM2/graph.hpp>
#include <planner/wppl/LaCAM2/utils.hpp>
#include <planner/wppl/RHCR/main/States.h>

namespace LaCAM2 {

    struct AgentInfo {
    public:
        int goal_location;
        float elapsed;
        float tie_breaker;
        int id;
        int stuck_order;
        bool disabled;

        AgentInfo() : id(-1), goal_location(-1), elapsed(-1), tie_breaker(-1), stuck_order(0), disabled(false){};
    };


    struct Instance {
        const Graph &G;// graph
        Config starts; // initial configuration
        Config goals;  // goal configuration
        const uint N;  // number of agents
        std::vector<AgentInfo> &agent_infos;
        int planning_window = -1;
        std::vector<RobotPath> *precomputed_paths;

        // // for testing
        // Instance(const std::string& map_filename,
        //          const std::vector<uint>& start_indexes,
        //          const std::vector<uint>& goal_indexes);
        // // for MAPF benchmark
        // Instance(const std::string& scen_filename, const std::string& map_filename,
        //          const uint _N = 1);
        // // random instance generation
        // Instance(const std::string& map_filename, std::mt19937* MT,
        //          const uint _N = 1);
        Instance(
                const Graph &G,
                const std::vector<std::pair<uint, int>> &start_indexes,
                const std::vector<std::pair<uint, int>> &goal_indexes,
                std::vector<AgentInfo> &agent_infos,
                int planning_window = -1,
                std::vector<RobotPath> *precomputed_paths = nullptr);
        ~Instance() {}

        void set_starts_and_goals(std::vector<State> *starts, std::vector<State> *goals);

        // simple feasibility check of instance
        bool is_valid(const int verbose = 0) const;
    };

    // solution: a sequence of configurations
    using Solution = std::vector<Config>;
    std::ostream &operator<<(std::ostream &os, const Solution &solution);

}// namespace LaCAM2
#endif
