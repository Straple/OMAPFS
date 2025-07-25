#include <planner/wppl/LNS/LNSSolver.h>
#ifdef ENABLE_ROTATE_MODEL

#include <planner/wppl/LNS/Parallel/DataStructure.h>
#include <planner/wppl/LNS/Parallel/GlobalManager.h>
#include <planner/wppl/util/Analyzer.h>
#include <planner/wppl/util/Dev.h>
#include <planner/wppl/util/Timer.h>

namespace LNS {

    LNSSolver::LNSSolver(
            const std::shared_ptr<HeuristicTable> &HT,
            Environment *env,
            std::shared_ptr<std::vector<float>> &map_weights,
            nlohmann::json &config,
            std::shared_ptr<LaCAM2::LaCAM2Solver> &lacam2_solver,
            int max_task_completed) : HT(HT),
                                      map_weights(map_weights),
                                      action_model(env),
                                      executor(env),
                                      slow_executor(env),
                                      config(config),
                                      MT(new std::mt19937(read_param_json<uint>(config, "seed", 0))),
                                      lacam2_solver(lacam2_solver),
                                      agent_infos(lacam2_solver->agent_infos),
                                      max_task_completed(max_task_completed){};


    void LNSSolver::initialize(const Environment &env) {
        // obstacle_stats_tree = std::make_shared<StatsTree>(env.cols,env.rows);

        // for (int pos=0;pos<env.map.size();++pos) {
        //     if (env.map[pos]==1) {
        //         obstacle_stats_tree->update(pos,1);
        //     }
        // }

        // agent_stats_tree = std::make_shared<StatsTree>(env.cols,env.rows);
        // lacam2_solver->initialize(env); // it is initialized already outside.
        execution_paths.resize(env.num_of_agents);
        planning_paths.resize(env.num_of_agents);

        // TODO(rivers): make it attribute of the class.
        int window_size_for_CT = read_param_json<int>(config, "window_size_for_CT");
        int window_size_for_PATH = read_param_json<int>(config, "window_size_for_PATH");
        int LaCAM2_planning_window = read_param_json<int>(config["LaCAM2"], "planning_window");
        if (window_size_for_CT == -1 || LaCAM2_planning_window != window_size_for_CT || window_size_for_PATH != window_size_for_CT) {
            cout << "not fully supported now! need to modify the padding path code in lns->" << endl;
            exit(-1);
        }

        planning_window = window_size_for_CT;                               // TODO(rivers): read from config & initialized in constructor
        execution_window = read_param_json<int>(config, "execution_window");// TODO(rivers): read from config & initialized in constructor
    }

    int get_neighbor_orientation(const Environment *env, int loc1, int loc2) {

        // 0:east, 1:south, 2:west, 3:north

        if (loc1 + 1 == loc2) {
            return 0;
        }

        if (loc1 + env->cols == loc2) {
            return 1;
        }

        if (loc1 - 1 == loc2) {
            return 2;
        }

        if (loc1 - env->cols == loc2) {
            return 3;
        }

        if (loc1 == loc2) {
            return 4;
        }

        return -1;
    }

    std::vector<State> subvector(std::vector<State> &v, int start, int end) {
        std::vector<State> ret;
        for (int i = start; i < end; ++i) {
            ret.push_back(v[i]);
        }
        return ret;
    }

    void LNSSolver::plan(const Environment &env, int time_limit) {
        // TODO(rivers): make it configurable.
        //double time_limit=read_param_json<double>(config,"cutoffTime");
        TimeLimiter time_limiter(time_limit * 0.001, env);

        ONLYDEV(g_timer.record_p("_plan_s");)

        ONLYDEV(g_timer.record_p("plan_s");)

        std::vector<State> starts;
        std::vector<State> goals;

        int disabled_agent_count = 0;
        for (int i = 0; i < env.num_of_agents; ++i) {
            if ((*agent_infos)[i].disabled) {
                ++disabled_agent_count;
            }
            starts.emplace_back(execution_paths[i].back().location, -1, execution_paths[i].back().orientation);
            if (!(i < env.goal_locations.size() && !env.goal_locations[i].empty())) {
                std::cout << "is empty" << std::endl;
            }
            goals.emplace_back(env.goal_locations[i][0], -1, -1);
        }

        ONLYDEV(std::cout << "disabled_agents:" << disabled_agent_count << std::endl;)

        // TODO(rivers): we need to replan for all agents that has no plan
        // later we may think of padding all agents to the same length
        if (planning_paths[0].size() < planning_window + 1) {
            // std::cout<<"call lacam2: "<<planning_paths[0].size()<<std::endl;
            // TODO(rivers): maybe we should directly build lacam2 planner in this class.
            if (read_param_json<string>(config, "initAlgo") == "LaCAM2") {
                ONLYDEV(g_timer.record_p("lacam2_plan_s");)
                // use lacam2 to get a initial solution
                // TODO(rivers): the following line may need to be optimized. There is no need to rebuild the graph G again.
                lacam2_solver->clear(env);

                // TODO(rivers): we should avoid copy here. we may use deque for paths.
                ONLYDEV(g_timer.record_p("copy_paths_1_s");)
                vector<RobotPath> precomputed_paths;
                precomputed_paths.resize(env.num_of_agents);
                for (int i = 0; i < env.num_of_agents; ++i) {
                    if (planning_paths[i][0].location != execution_paths[i].back().location || planning_paths[i][0].orientation != execution_paths[i].back().orientation) {
                        cout << "agent " << i << "'s current state doesn't match with the plan" << endl;// TODO: modify this cout.
                        exit(-1);
                    }
                    for (int j = 0; j < planning_paths[i].size(); ++j) {
                        precomputed_paths[i].emplace_back(planning_paths[i][j]);
                        // we could break if we arrive at goal eariler here.
                        if (j > 1 && planning_paths[i][j].location == env.goal_locations[i][0]) {
                            break;
                        }
                    }
                    // std::cout<<"agent "<<i<<std::endl;
                    // std::cout<<planning_paths[i]<<std::endl;
                    // std::cout<<precomputed_paths[i]<<std::endl;
                }
                ONLYDEV(g_timer.record_d("copy_paths_1_s", "copy_paths_1_e", "copy_paths_1");)


                // TODO: lacam2_solver should plan with starts differnt from env.curr_states but goals the same as env.goals because they are up-to-date.
                lacam2_solver->plan(env, &precomputed_paths, &starts, &goals);
                // cout<<"lacam succeed"<<endl;

                // we need to copy the new planned paths into paths
                // std::cout<<"lacam2 paths:"<<endl;
                // for (int i=0;i<env.num_of_agents;++i) {
                //     std::cout<<"before agent "<<i<<" "<<env.curr_states[i]<<"->"<<env.goal_locations[i][0].first<<" "<<planning_paths[i].size()<<": "<<planning_paths[i]<<std::endl;
                //     std::cout<<lacam2_solver->paths[i]<<endl;
                // }
                int num_inconsistent = 0;
                for (int i = 0; i < env.num_of_agents; ++i) {
                    for (int j = 0; j < planning_paths[i].size() - 1; ++j) {
                        if (planning_paths[i][j].location != lacam2_solver->paths[i][j].location || planning_paths[i][j].orientation != lacam2_solver->paths[i][j].orientation) {
                            ++num_inconsistent;
                            break;
                        }
                    }

                    planning_paths[i].clear();
                    for (int j = 0; j < lacam2_solver->paths[i].size(); ++j) {
                        planning_paths[i].emplace_back(lacam2_solver->paths[i][j].location, j, lacam2_solver->paths[i][j].orientation);
                    }
                    // std::cout<<"agent "<<i<<" "<<env.curr_states[i]<<"->"<<env.goal_locations[i][0].first<<" "<<planning_paths[i].size()<<": "<<planning_paths[i]<<std::endl;
                }
                ONLYDEV(std::cout << "num_inconsistent/total: " << num_inconsistent << "/" << env.num_of_agents << "=" << num_inconsistent / (float) env.num_of_agents << std::endl;)
                ONLYDEV(g_timer.record_d("lacam2_plan_s", "lacam2_plan_e", "lacam2_plan");)
            }

            // ONLYDEV(analyzer.snapshot(
            //     "analysis/ppaths/lacam2",
            //     executed_plan_step,
            //     paths
            // );)
        }


        // TODO(rivers): not sure what's bug making it cannot be placed in initialize()
        ONLYDEV(g_timer.record_p("prepare_LNS_s");)
        if (env.curr_timestep == 0) {
            // build instace
            instance = std::make_shared<Instance>(env);
            lns = std::make_shared<Parallel::GlobalManager>(
                    true,
                    *instance,
                    HT,
                    map_weights,
                    agent_infos,
                    read_param_json<int>(config, "neighborSize"),
                    Parallel::destroy_heuristic::RANDOMWALK,// TODO: always randomwalk
                    true,                                   // TODO: always Adaptive
                    0.01,                                   // TODO: decay factor
                    0.01,                                   // TODO: reaction factor
                    read_param_json<string>(config, "initAlgo"),
                    read_param_json<string>(config, "replanAlgo"),
                    false,// TODO: not sipp
                    read_param_json<int>(config, "window_size_for_CT"),
                    read_param_json<int>(config, "window_size_for_CAT"),
                    read_param_json<int>(config, "window_size_for_PATH"),
                    execution_window,
                    lacam2_solver->max_agents_in_use != env.num_of_agents,// TODO: has disabled agents
                    read_param_json<bool>(config, "fix_ng_bug"),
                    0// TODO: screen
            );
        }

        lns->reset();
        instance->set_starts_and_goals(starts, goals);
        ONLYDEV(g_timer.record_d("prepare_LNS_s", "prepare_LNS_e", "prepare_LNS");)

        // ONLYDEV(g_timer.record_p("prepare_LNS_s");)
        // build planner
        // PIBTPPS_option pipp_option;
        // pipp_option.windowSize = read_param_json<int>(config,"pibtWindow");
        // pipp_option.winPIBTSoft = read_param_json<int>(config,"winPibtSoftmode");

        // lns = new LNS(
        //     *instance,
        //     read_param_json<double>(config,"cutoffTime"),
        //     read_param_json<string>(config,"initAlgo"),
        //     read_param_json<string>(config,"replanAlgo"),
        //     read_param_json<string>(config,"destoryStrategy"),
        //     read_param_json<int>(config,"neighborSize"),
        //     read_param_json<int>(config,"maxIterations"),
        //     read_param_json<bool>(config,"initLNS"),
        //     read_param_json<string>(config,"initDestoryStrategy"),
        //     read_param_json<bool>(config,"sipp"),
        //     read_param_json<int>(config,"screen"),
        //     pipp_option,
        //     HT,
        //     read_param_json<int>(config,"window_size_for_CT"),
        //     read_param_json<int>(config,"window_size_for_CAT"),
        //     read_param_json<int>(config,"window_size_for_PATH")
        // );

        // ONLYDEV(g_timer.record_d("prepare_LNS_s","prepare_LNS_e","prepare_LNS");)

        // ONLYDEV(g_timer.record_p("modify_goals_s");)
        // TODO(rivers): this might not be necessary
        // modify_goals(instance.goal_locations, env);
        // ONLYDEV(g_timer.record_d("modify_goals_s","modify_goals_e","modify_goals");)

        // copy current paths to lns paths
        // we need to do this every timestep because the goal might be updated.
        ONLYDEV(g_timer.record_p("copy_paths_2_s");)
        for (int i = 0; i < lns->agents.size(); i++) {
            if (lns->agents[i].id != i) {
                cout << "agents are not ordered at the begining" << endl;
                exit(-1);
            }
            lns->agents[i].path.clear();
            bool goal_arrived = false;
            for (int j = 0; j < planning_paths[i].size(); ++j) {
                lns->agents[i].path.nodes.emplace_back(planning_paths[i][j].location, planning_paths[i][j].orientation);
                if (planning_paths[i][j].location == env.goal_locations[i][0]) {
                    goal_arrived = true;
                    // break;
                }
            }
            // TODO(rivers): it is not correct on weighted maps
            lns->agents[i].path.path_cost = lns->agents[i].getEstimatedPathLength(lns->agents[i].path, env.goal_locations[i][0], HT);
            // cout<<"agent "<<i<<": ";
            // for (int j=0;j<lns->agents[i].path.size();++j){
            //     cout<<lacam2_solver->paths[i][j].location<<" ";
            // }
            // cout<<endl;
        }
        ONLYDEV(g_timer.record_d("copy_paths_2_s", "copy_paths_2_e", "copy_paths_2");)

        ONLYDEV(g_timer.record_p("run_LNS_s");)
        // continue optimizing paths
        bool succ = lns->run(time_limiter);
        // if (succ)
        // {
        //     cout<<"lns succeed"<<endl;
        // } else {
        //     cout<<"lns failed"<<endl;
        //     exit(-1);
        // }

        // we cannot do this because it would make result invalid
        // deal with a special case when the goal and the start are the same.
        if (execution_window == 1) {
            for (int i = 0; i < lns->agents.size(); ++i) {
                if (lns->agents[i].path.size() < planning_window + 1) {
                    // in this case, actually the goal is the same as the start
                    lns->agents[i].path.nodes.resize(planning_window + 1, lns->agents[i].path.back());
                }
            }
        }
        ONLYDEV(g_timer.record_d("run_LNS_s", "run_LNS_e", "run_LNS");)

        // save to paths
        ONLYDEV(g_timer.record_p("copy_paths_3_s");)
        // std::cout<<"lns paths:"<<endl;
        for (int i = 0; i < planning_paths.size(); ++i) {
            auto &path = planning_paths[i];
            auto &new_path = lns->agents[i].path;

            // compare
            // for (int j=1;j<new_path.size();++j) {
            //     if (path[executed_plan_step+j].location!=new_path[j].location || path[executed_plan_step+j].orientation!=new_path[j].orientation){
            //         std::cout<<"agent "<<i<<" has updated lns path"<<endl;
            //         std::cout<<subvector(path,executed_plan_step,path.size())<<endl;
            //         std::cout<<new_path<<endl;
            //         // std::cout<<lns->agents[i].path<<endl;
            //         break;
            //     }
            // }

            // std::cout<<"agent "<<i<<" has updated lns path"<<endl;
            // std::cout<<path<<endl;
            // std::cout<<new_path<<endl;

            path.clear();

            // cout<<"agent "<<i<<" "<<new_path.size()<<": "<<new_path<<endl;
            for (int j = 0; j < new_path.size(); ++j) {
                path.emplace_back(new_path[j].location, j, new_path[j].orientation);
                // if (need_new_execution_paths && new_path[j].location==env.goal_locations[i][0].first){
                //     break;
                // }
            }
            // cout<<"agent "<<i<<" s:"<<env.curr_states[i]<<" e:"<<env.goal_locations[i][0].first<<" c:"<<executed_plan_step<<endl;
            // std::cout<<"agent "<<i<<" "<<env.curr_states[i]<<"->"<<env.goal_locations[i][0].first<<" "<<path.size()<<": "<<path<<endl;
            // std::cout<<path<<endl;
        }
        ONLYDEV(g_timer.record_d("copy_paths_3_s", "copy_paths_3_e", "copy_paths_3");)

        // for (int i=0;i<paths.size();++i){
        //     cout<<executed_plan_step<<" "<<env.curr_states[i].location<<" "<<env.curr_states[i].orientation<<endl;
        //     cout<<"agent "<<i<<": ";
        //     for (int j=0;j<paths[i].size();++j){
        //         cout<<paths[i][j].location<<" ";
        //     }
        //     cout<<endl;
        // }
        ONLYDEV(g_timer.record_d("plan_s", "plan_e", "plan");)

        ONLYDEV(
                double plan_time = g_timer.record_d("_plan_s", "_plan_e", "_plan");
                if (plan_time > max_plan_time) {
                    max_plan_time = plan_time;
                } std::cout
                << "max_plan_time: " << max_plan_time << endl;
                g_timer.remove_d("_plan");
                // if (plan_time>1.0){
                //     exit(-1);
                // }
        )
    }

    void LNSSolver::observe(const Environment &env) {
        // for (int i=0;i<env.num_of_agents;++i) {
        //     paths[i].clear();
        // }

        ONLYDEV(g_timer.record_p("observe_s");)

        if (execution_paths[0].size() == 0) {
            // the first step?
            for (int i = 0; i < env.num_of_agents; ++i) {
                execution_paths[i].push_back(env.curr_states[i]);
                planning_paths[i].push_back(env.curr_states[i]);
                // std::cout<<"agent "<<i<<" "<<env.curr_states[i]<<"->"<<env.goal_locations[i][0].first<<std::endl;
            }
            executed_step = 0;
        } else {
            bool match = true;
            for (int i = 0; i < execution_paths.size(); ++i) {
                // cout<<"agent "<<i<<" curr state:"<<env.curr_states[i]<<", "<<" goal:"<<env.goal_locations[i][0].first<<endl;
                if (executed_step + 1 >= execution_paths[i].size()) {
                    cout << "executed_step exceed the execution paths:" << executed_step << execution_paths[i].size() << endl;
                    exit(-1);
                }
                if (execution_paths[i][executed_step + 1].location != env.curr_states[i].location || execution_paths[i][executed_step + 1].orientation != env.curr_states[i].orientation) {
                    match = false;
                }
            }
            // otherwise, the previous execution is delayed.
            if (match) {
                ++executed_step;
            }
        }

        // if run out of execution instructions, we need to copy new ones into execution_paths
        if (executed_step == execution_paths[0].size() - 1) {
            // std::cout<<"need new execution paths"<<std::endl;
            need_new_execution_paths = true;
        } else {
            need_new_execution_paths = false;
        }

        ONLYDEV(g_timer.record_d("observe_s", "observe_e", "observe");)
    }

    void LNSSolver::get_step_actions(const Environment &env, vector<ActionType> &actions) {
        ONLYDEV(g_timer.record_p("get_step_actions_s");)

        assert(actions.empty());

#ifndef NO_ROT
        // get current state and current timestep
        // vector<State> planned_next_states;
        // vector<State> next_states;
        // int next_plan_step=executed_plan_step+1;
        // for (int i=0;i<env.num_of_agents;++i) {
        //     if (next_plan_step>=paths[i].size()){
        //         // todo: we need to wait for the new plan to come out.
        //         exit(-1);
        //     }
        //     planned_next_states.emplace_back(paths[i][next_plan_step].location,-1,-1);
        //     next_states.emplace_back(-1,-1,-1);
        // }

        // slow_executor.execute(&(env.curr_states),&planned_next_states,&next_states);

        // for (int i=0;i<env.num_of_agents;++i) {
        //     if (next_states[i].timestep!=env.curr_states[i].timestep+1) {
        //         std::cout<<"agent "<<i<<"'s plan doesn't show consecutive timesteps: "<<next_states[i].timestep<<" "<<env.curr_states[i].timestep<<endl;
        //         exit(-1);
        //     }
        // }


        if (need_new_execution_paths) {
            // copy planning paths to execution paths
            for (int i = 0; i < env.num_of_agents; ++i) {
                execution_paths[i].clear();
                for (int j = 0; j < execution_window + 1; ++j) {
                    execution_paths[i].emplace_back(planning_paths[i][j].location, j, planning_paths[i][j].orientation);
                }
                if (execution_paths[i].size() != execution_window + 1) {
                    std::cout << "execution paths size is not correct" << std::endl;
                    exit(-1);
                }
                executed_step = 0;
            }

            // keep only the remaining planning paths
            // std::cout<<"truncated planning paths"<<std::endl;
            for (int i = 0; i < env.num_of_agents; ++i) {
                planning_paths[i].erase(planning_paths[i].begin(), planning_paths[i].begin() + execution_window);
                if (planning_paths[i].size() != planning_window + 1 - execution_window) {
                    std::cout << "planning paths size is not correct" << std::endl;
                    exit(-1);
                }
                // std::cout<<"agent "<<i<<": "<<planning_paths[i]<<std::endl;
            }
        }


        if (num_task_completed >= max_task_completed) {// only for competition purpose, don't reveal too much information, otherwise it is too tired to overfit... do something fun instead!
            for (int i = 0; i < env.num_of_agents; ++i) {
                actions.push_back(ActionType::WAIT);
            }
        } else {
            // get actions from current state and next state
            for (int i = 0; i < env.num_of_agents; ++i) {
                // we will get action indexed at executed_plan_step+1
                if (execution_paths[i].size() <= executed_step + 1) {
                    cout << "wierd error for agent " << i << ". path length: " << execution_paths[i].size() << ", "
                         << "executed_plan_step+1: " << executed_step + 1 << endl;
                    exit(-1);
                }

                if (execution_paths[i][executed_step].location != env.curr_states[i].location || execution_paths[i][executed_step].orientation != env.curr_states[i].orientation) {
                    cout << "agent " << i << "'s current state doesn't match with the executed plan" << endl;
                    exit(-1);
                }

                actions.push_back(get_action_from_states(execution_paths[i][executed_step], execution_paths[i][executed_step + 1]));

                // assume perfect execution
                if (execution_paths[i][executed_step + 1].location == env.goal_locations[i][0]) {
                    ++num_task_completed;
                }
            }
        }
#else

        for (int i = 0; i < env.num_of_agents; ++i) {
            // we will get action indexed at executed_plan_step+1
            if (paths[i].size() <= executed_plan_step + 1) {
                cout << "wierd error for agent " << i << ". path length: " << paths[i].size() << ", "
                     << "executed_plan_step+1: " << executed_plan_step + 1 << endl;
                assert(false);
            }
            actions.push_back(get_action_from_states(paths[i][executed_plan_step], paths[i][executed_plan_step + 1]));
        }

#endif

        ONLYDEV(
                if (!action_model.is_valid(env.curr_states, actions)) {
                    cout << "planed actions are not valid in executed_step " << executed_step + 1 << "!" << endl;
                    for (int i = 0; i < env.num_of_agents; ++i) {
                        cout << "agent " << i << " " << env.curr_states[i] << " " << actions[i] << endl;
                    }
                    exit(-1);
                })

        ONLYDEV(g_timer.record_d("get_step_actions_s", "get_step_actions_e", "get_step_actions");)
    }

}// end namespace LNS
#endif
