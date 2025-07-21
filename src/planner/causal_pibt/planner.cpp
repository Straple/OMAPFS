#ifdef ENABLE_ROTATE_MODEL

#include <environment/environment.hpp>
#include <planner/causal_pibt/flow.hpp>
#include <planner/causal_pibt/heuristics.hpp>
#include <planner/causal_pibt/pibt.hpp>
#include <planner/causal_pibt/planner.hpp>

#include <environment/info.hpp>

#include <random>

namespace CausalPlanner {
    extern std::vector<HeuristicTable> global_heuristictable;
    extern Neighbors global_neighbors;
};// namespace CausalPlanner

using namespace CausalPlanner;

/**
 * @brief Default planner initialization
 *
 * @param preprocess_time_limit time limit for preprocessing in milliseconds
 * @param env shared environment object
 *
 * The initialization function initializes the default planner data structures and heuristics tables.
 */
void CausalPIBT::initialize(Environment *env) {
    //initialise all required data structures
    assert(env->num_of_agents != 0);
    p.assign(env->num_of_agents, 0);
    decision.assign(env->map.size(), -1);
    prev_decision.assign(env->map.size(), -1);
    prev_states.assign(env->num_of_agents, {});
    next_states.resize(env->num_of_agents);
    decided.assign(env->num_of_agents, DCR({-1, DONE::DONE}));
    occupied.assign(env->map.size(), false);
    checked.assign(env->num_of_agents, false);
    ids.assign(env->num_of_agents, {});
    require_guide_path.resize(env->num_of_agents, false);
    for (int i = 0; i < ids.size(); i++) {
        ids[i] = i;
    }

    new (&trajLNS) TrajLNS(env, global_heuristictable, global_neighbors);
    trajLNS.init_mem();

    //assign intial priority to each agent
    std::shuffle(ids.begin(), ids.end(), std::mt19937(0));
    for (int i = 0; i < ids.size(); i++) {
        p[ids[i]] = ((double) (ids.size() - i)) / ((double) (ids.size() + 1));
    }
    p_copy = p;
}

/**
     * @brief Default planner plan function
     *
     * @param time_limit time limit for planning in milliseconds
     * @param actions vector of actions to be populated by the planner
     * @param env shared environment object
     *
     * The plan function is the main function of the default planner.
     * It computes the actions for the agents based on the current state of the environment.
     * The function first checks assignments/goal location changes and perform the necessary updates.
     * It then computes and optimises traffic flow optimised guide paths for the agents.
     * Finally, it computes the actions for the agents using PIBT that follows the guide path heuristics and returns the actions.
     * Note that the default planner ignores the turning action costs, and post-processes turning actions as additional delays on top of original plan.
     */
void CausalPIBT::plan(TimePoint end_time, std::vector<ActionType> &actions, Environment *env) {

    // recrod the initial location of each agent as dummy goals in case no goal is assigned to the agent.
    if (env->curr_timestep == 0) {
        dummy_goals.resize(env->num_of_agents);
        for (int i = 0; i < env->num_of_agents; i++) {
            dummy_goals.at(i) = env->curr_states.at(i).location;
        }
    }

    // data sturcture for record the previous decision of each agent
    prev_decision.assign(env->map.size(), -1);

    // update the status of each agent and prepare for planning
    int count = 0;
    for (int i = 0; i < env->num_of_agents; i++) {
        //initialise the shortest distance heuristic table for the goal location of the agent
        if (get_now() < end_time) {
            for (int j = 0; j < env->goal_locations[i].size(); j++) {
                int goal_loc = env->goal_locations[i][j];
                if (trajLNS.heuristics.at(goal_loc).empty()) {
                    init_heuristic(trajLNS.heuristics[goal_loc], env, goal_loc);
                    count++;
                }
            }
        }

        // set the goal location of each agent
        if (env->goal_locations[i].empty()) {
            trajLNS.tasks[i] = dummy_goals.at(i);
            p[i] = p_copy[i];
        } else {
            trajLNS.tasks[i] = env->goal_locations[i].front();
        }

        // check if the agent need a guide path update, when the agent has no guide path or the guide path does not end at the goal location
        require_guide_path[i] = false;
        if (trajLNS.trajs[i].empty() || trajLNS.trajs[i].back() != trajLNS.tasks[i])
            require_guide_path[i] = true;

        // check if the agent completed the action in the previous timestep
        // if not, the agent is till turning towards the action direction, we do not need to plan new action for the agent
        assert(env->curr_states[i].location >= 0);
        prev_states[i] = env->curr_states[i];
        next_states[i] = State();
        prev_decision[env->curr_states[i].location] = i;
        if (decided[i].loc == -1) {
            decided[i].loc = env->curr_states[i].location;
            assert(decided[i].state == DONE::DONE);
        }
        if (prev_states[i].location == decided[i].loc) {
            decided[i].state = DONE::DONE;
        }
        if (decided[i].state == DONE::NOT_DONE) {
            decision.at(decided[i].loc) = i;
            next_states[i] = State(decided[i].loc, -1, -1);
        }

        // reset the pibt priority if the agent reached previous goal location and switch to new goal location
        if (require_guide_path[i])
            p[i] = p_copy[i];
        else if (!env->goal_locations[i].empty())
            p[i] = p[i] + 1;

        // give priority bonus to the agent if the agent is in a deadend location
        if (!env->goal_locations[i].empty() && trajLNS.neighbors[env->curr_states[i].location].size() == 1) {
            p[i] = p[i] + 10;
        }
    }

    if (get_planner_type() == PlannerType::PIBT_TF) {
        // compute the congestion minimised guide path for the agents that need guide path update
        for (int i = 0; i < env->num_of_agents; i++) {
            if (get_now() >= end_time)
                break;
            if (require_guide_path[i]) {
                if (!trajLNS.trajs[i].empty())
                    remove_traj(trajLNS, i);
                update_traj(trajLNS, i);
            }
        }

        // iterate and recompute the guide path to optimise traffic flow
        std::unordered_set<int> updated;
        frank_wolfe(trajLNS, updated, end_time);
    }

    // sort agents based on the current priority
    std::sort(ids.begin(), ids.end(), [&](int a, int b) {
        return p.at(a) > p.at(b);
    });

    // compute the targeted next location for each agent using PIBT
    for (int i: ids) {
        if (decided[i].state == DONE::NOT_DONE) {
            continue;
        }
        if (next_states[i].location == -1) {
            assert(prev_states[i].location >= 0 && prev_states[i].location < env->map.size());
            causalPIBT(i, -1, prev_states, next_states,
                       prev_decision, decision,
                       occupied, trajLNS);
        }
    }

    // post processing the targeted next location to turning or moving actions
    actions.resize(env->num_of_agents);
    for (int id: ids) {
        //clear the decision table based on which agent has next_states
        if (next_states.at(id).location != -1)
            decision.at(next_states.at(id).location) = -1;
        // if agent is newly assigned a targeted next location, record the decision as not done yet
        if (next_states.at(id).location >= 0) {
            decided.at(id) = DCR({next_states.at(id).location, DONE::NOT_DONE});
        }

        // post process the targeted next location to turning or moving actions
        actions.at(id) = getAction(prev_states.at(id), decided.at(id).loc, env);
        checked.at(id) = false;
    }

    // recursively check if the FW action can be executed by checking whether all agents in the front of the agent can move forward
    // if any agent cannot move foward due to turning, all agents behind the turning agent will not move forward.
    for (int id = 0; id < env->num_of_agents; id++) {
        if (!checked.at(id) && actions.at(id) == ActionType::FORWARD) {
            moveCheck(id, checked, decided, actions, prev_decision);
        }
    }

    prev_states = next_states;
}

#endif