#pragma once

#ifndef NO_ROT

#include <ctime>
#include <environment/action_model.hpp>
#include <nlohmann/json.hpp>
#include <planner/causal_pibt/environment.hpp>
#include <planner/wppl/RHCR/interface/CompetitionGraph.h>
#include <planner/wppl/RHCR/interface/RHCRSolver.h>
#include <planner/wppl/RHCR/main/ECBS.h>
#include <planner/wppl/RHCR/main/ID.h>
#include <planner/wppl/RHCR/main/LRAStar.h>
#include <planner/wppl/RHCR/main/MAPFSolver.h>
#include <planner/wppl/RHCR/main/PBS.h>
#include <planner/wppl/RHCR/main/SingleAgentSolver.h>
#include <planner/wppl/RHCR/main/WHCAStar.h>
//#include "common.h"
#include <memory>
#include <planner/wppl/LNS/LNSSolver.h>
#include <planner/wppl/LaCAM2/LaCAM2Solver.hpp>

class WPPL {
public:
    DefaultPlanner::SharedEnvironment *env = nullptr;

    void initialize(DefaultPlanner::SharedEnvironment* new_env);

    // return next states for all agents
    void plan(int time_limit, std::vector<ActionType> &plan);

    // Start kit dummy implementation
    std::list<pair<int, int>> single_agent_plan(int start, int start_direct, int end);
    int getManhattanDistance(int loc1, int loc2);
    std::list<pair<int, int>> getNeighbors(int location, int direction);
    bool validateMove(int loc, int loc2);

    bool consider_rotation = true;
    string lifelong_solver_name;
    std::shared_ptr<RHCR::RHCRSolver> rhcr_solver;
    std::shared_ptr<LaCAM2::LaCAM2Solver> lacam2_solver;
    std::shared_ptr<LNS::LNSSolver> lns_solver;

    double max_step_time = 0;

    std::shared_ptr<std::vector<float>> map_weights;
    nlohmann::json config;
    void load_configs();
    std::string load_map_weights(string weights_path);

    RHCR::MAPFSolver *rhcr_build_mapf_solver(nlohmann::json &config, RHCR::CompetitionGraph &graph);
    void rhcr_config_solver(std::shared_ptr<RHCR::RHCRSolver> &solver, nlohmann::json &config);

    int max_execution_steps;
};

#else

#include "ActionModel.h"
#include "LNS/LNSSolver.h"
#include "LaCAM2/LaCAM2Solver.hpp"
#include "SharedEnv.h"
#include "common.h"
#include "nlohmann/json.hpp"
#include <ctime>
#include <memory>

class MAPFPlanner {
public:
    DefaultPlanner::SharedEnvironment *env;

    MAPFPlanner(DefaultPlanner::SharedEnvironment *env) : env(env){};
    MAPFPlanner() { env = new DefaultPlanner::SharedEnvironment(); };
    virtual ~MAPFPlanner() { delete env; };


    virtual void initialize(int preprocess_time_limit);

    // return next states for all agents
    virtual void plan(int time_limit, std::vector<ActionType> &plan);

    // Start kit dummy implementation
    std::list<pair<int, int>> single_agent_plan(int start, int start_direct, int end);
    int getManhattanDistance(int loc1, int loc2);
    std::list<pair<int, int>> getNeighbors(int location, int direction);
    bool validateMove(int loc, int loc2);

    bool consider_rotation = true;
    string lifelong_solver_name;
    std::shared_ptr<LaCAM2::LaCAM2Solver> lacam2_solver;
    std::shared_ptr<LNS::LNSSolver> lns_solver;

    std::shared_ptr<std::vector<int>> map_weights;
    nlohmann::json config;
    void load_configs();
    std::string load_map_weights(string weights_path);
};

#endif