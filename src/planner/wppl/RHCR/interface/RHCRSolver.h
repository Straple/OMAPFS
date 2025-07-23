#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <environment/action_model.hpp>
#include <environment/environment.hpp>
#include <planner/wppl/RHCR/interface/CompetitionGraph.h>
#include <planner/wppl/RHCR/main/BasicSystem.h>
#include <planner/wppl/RHCR/main/MAPFSolver.h>
#include <planner/wppl/util/CompetitionActionModel.h>

#include <future>
#include <mutex>
#include <thread>

namespace RHCR {
    class RHCRSolver : public BasicSystem {
        // for simplicity, just make everything public. but attributes or functions start with _ are supposed to use privately in general cases.
    public:
        //***** attributes *****//
        // std::thread task;
        // std::future<void()> future;
        // this deque maintains actions planned into future. we can directly retrieve the actions for the current step from this deque.
        // TODO it should be protected by a mutex
        // bool stop_flag;

        CompetitionGraph &graph;
        CompetitionActionModelWithRotate model;
        bool need_replan = true;
        int total_feasible_timestep = 0;

        //***** functions *****//
        RHCRSolver(CompetitionGraph &graph, MAPFSolver &mapf_solver, Environment *env) : BasicSystem(graph, mapf_solver), graph(graph), model(env){};

        void start_plan_task();
        void stop_plan_task();

        // this function plans actions into future (possible from a certain state?)
        void plan(const Environment &env);

        void get_step_actions(const Environment &env, vector<ActionType> &actions);

        // inline void set_mapf_solver(const shared_ptr<MAPFSolver> & mapf_solver){
        //     this->mapf_solver=mapf_solver;
        // };

        // inline void set_map(const shared_ptr<BasicGraph> & map){
        //     this->map=map;
        // };

        void initialize(const Environment &env);
        void update_goal_locations(const Environment &env);
        void set_parameters(const string &map_name);
    };
}// namespace RHCR
#endif
