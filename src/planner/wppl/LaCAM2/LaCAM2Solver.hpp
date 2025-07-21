#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <planner/wppl/LaCAM2/executor.hpp>
#include <planner/wppl/LaCAM2/graph.hpp>
#include <planner/wppl/LaCAM2/instance.hpp>
#include <planner/wppl/LaCAM2/planner.hpp>
#include <planner/wppl/LaCAM2/slow_executor.hpp>
#include <planner/wppl/util/CompetitionActionModel.h>
#include <planner/wppl/util/HeuristicTable.h>
#include <planner/wppl/util/common.h>
#include <random>

namespace LaCAM2 {

    class LaCAM2Solver {

    public:
        // [start] the following lines should be abstracted away.
        std::vector<RobotPath> paths;
        CompetitionActionModelWithRotate action_model;
        std::mt19937 *MT;// seed for randomness
        bool need_replan = true;
        int total_feasible_timestep = 0;
        int timestep = 0;
        void initialize(const Environment &env);
        void plan(const Environment &env, std::vector<RobotPath> *precomputed_paths = nullptr, std::vector<State> *starts = nullptr, std::vector<State> *goals = nullptr);
        void get_step_actions(const Environment &env, vector<ActionType> &actions);
        // ActionType get_action_from_states(const State & state, const State & next_state);
        // [end]

        bool flag = false;

        std::shared_ptr<Graph> G;                       // graph
        std::shared_ptr<HeuristicTable> HT;             // instance
        std::shared_ptr<std::vector<float>> map_weights;// map weights

        std::shared_ptr<std::vector<AgentInfo>> agent_infos;

        int max_agents_in_use;
        bool disable_corner_target_agents;
        bool disable_agent_goals;

        int execution_window;
        int planning_window;

        bool use_external_executor = false;

        int num_task_completed = 0;
        int max_task_completed;

        // Config next_config;

        // std::vector<std::vector<int>> action_costs;
        // std::vector<std::vector<int>> total_actions;

        Executor executor;
        SlowExecutor slow_executor;

        nlohmann::json config;

        Instance build_instance(const Environment &env, std::vector<RobotPath> *precomputed_paths = nullptr);

        float get_action_cost(int pst, int ost, int ped, int oed);
        float eval_solution(const Instance &instance, const Solution &solution);

        int get_neighbor_orientation(int loc1, int loc2);

        void solution_convert(const Environment &env, Solution &solution, std::vector<RobotPath> &paths);

        LaCAM2Solver(const std::shared_ptr<HeuristicTable> &HT, Environment *env, std::shared_ptr<std::vector<float>> &map_weights, int max_agents_in_use, bool disable_corner_target_agents,
                     int max_task_completed,
                     nlohmann::json &config) : HT(HT),
                                               map_weights(map_weights),
                                               action_model(env), executor(env), slow_executor(env),
                                               config(config),
                                               MT(new std::mt19937(read_param_json<uint>(config, "seed", 0))),
                                               max_agents_in_use(max_agents_in_use),
                                               disable_corner_target_agents(disable_corner_target_agents),
                                               max_task_completed(max_task_completed) {

            use_external_executor = read_param_json<bool>(config, "use_external_executor");
            planning_window = read_param_json<int>(config, "planning_window");
            execution_window = read_param_json<int>(config, "execution_window");
            disable_agent_goals = "NONE";//read_param_json<bool>(config, "disable_agent_goals");
        };

        ~LaCAM2Solver() {
            delete MT;
        };


        void clear(const Environment &env) {
            int num_of_agents = paths.size();

            paths.clear();
            paths.resize(env.num_of_agents);

            need_replan = true;
            total_feasible_timestep = 0;
            timestep = 0;
            delete MT;
            MT = new std::mt19937(read_param_json(config, "seed", 0));

            for (int i = 0; i < env.num_of_agents; ++i) {
                auto &agent_info = (*agent_infos)[i];
                agent_info.goal_location = -1;
                agent_info.elapsed = -1;
                agent_info.tie_breaker = -1;
                agent_info.stuck_order = 0;
            }


            // next_config = Config();
            // agent_infos.clear();

            // initialize(env);
        }

        void disable_agents(const Environment &env);

        ActionType get_action_from_states(const State &state, const State &next_state) {
#ifndef NO_ROT
            // assert(state.timestep+1==next_state.timestep);

            if (state.location == next_state.location) {
                // either turn or wait
                if (state.orientation == next_state.orientation) {
                    return ActionType::WAIT;
                } else if ((state.orientation - next_state.orientation + 4) % 4 == 3) {
                    return ActionType::ROTATE;
                } else if ((state.orientation - next_state.orientation + 4) % 4 == 1) {
                    return ActionType::COUNTER_ROTATE;
                } else {
                    assert(false);
                    return ActionType::WAIT;
                }
            } else {
                return ActionType::FORWARD;
            }
#else
            assert(state.timestep + 1 == next_state.timestep);

            for (int i = 0; i < 5; ++i) {
                if (state.location + action_model.moves[i] == next_state.location) {
                    return static_cast<ActionType>(i);
                }
            }

            cout << "Cannot get action from invalid movement between state " << state << " and " << next_state << endl;
            exit(-1);
#endif
        }
    };

}// namespace LaCAM2