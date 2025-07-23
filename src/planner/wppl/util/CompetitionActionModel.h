#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <environment/environment.hpp>
#include <environment/action_model.hpp>

#include <string>
#include <list>

#ifndef NO_ROT

// TODO(hj): move it outside the RHCR
class CompetitionActionModelWithRotate {
public:
    std::list<std::tuple<std::string, int, int, int>> errors;

    CompetitionActionModelWithRotate(){};
    CompetitionActionModelWithRotate(Environment *env) : env(env), rows(env->rows), cols(env->cols) {
        moves[0] = 1;
        moves[1] = cols;
        moves[2] = -1;
        moves[3] = -cols;
    };

    bool is_valid(const std::vector<State> &prev, const std::vector<ActionType> &action);
    void set_logger(/*Logger* logger*/) {
        //this->logger = logger;
    }

    std::vector<State> result_states(const std::vector<State> &prev, const std::vector<ActionType> &action) {
        std::vector<State> next(prev.size());
        for (size_t i = 0; i < prev.size(); i++) {
            next[i] = result_state(prev[i], action[i]);
        }
        return next;
    };

    std::vector<State> get_state_neighbors(const State &curr, bool no_forward) {
        std::vector<State> neighbors;

        int x = curr.location % env->cols;
        int y = curr.location / env->cols;

        if (!no_forward) {
            const auto &next_state = result_state(curr, ActionType::FORWARD, true);

            // if won't go out of map
            if (next_state.location != -1) {
                neighbors.push_back(result_state(curr, ActionType::FORWARD));
            }
        }
        neighbors.push_back(result_state(curr, ActionType::ROTATE));
        neighbors.push_back(result_state(curr, ActionType::COUNTER_ROTATE));
        neighbors.push_back(result_state(curr, ActionType::WAIT));

        return neighbors;
    }

    std::vector<State> get_loc_neighbors(const State &curr, bool move_orient = true) {
        std::vector<State> neighbors;

        int x = curr.location % env->cols;
        int y = curr.location / env->cols;

        for (int i = 0; i < 4; ++i) {
            int new_location = curr.location + moves[i];
            int new_orientation;

            if (move_orient) {
                new_orientation = i;
            } else {
                new_orientation = 0;
            }

            if ((x == env->cols - 1 && i == 0) || (y == env->rows - 1 && i == 1) || (x == 0 && i == 2) || (y == 0 && i == 3)) {
                continue;
            }

            // if in the map but blocked
            if (env->map[new_location]) {
                continue;
            }

            neighbors.push_back(State(new_location, curr.timestep + 1, new_orientation));
        }

        return neighbors;
    }

    Environment *env = nullptr;
    int rows;
    int cols;
    int moves[4];
    //Logger* logger = nullptr;

    State result_state(const State &prev, ActionType action, bool check = false) {
        int new_location = prev.location;
        int new_orientation = prev.orientation;
        if (action == ActionType::FORWARD) {
            new_location = new_location += moves[prev.orientation];
            if (check) {
                int x = prev.location % env->cols;
                int y = prev.location / env->cols;

                int nx = new_location % env->cols;
                int ny = new_location / env->cols;

                if ((x == env->cols - 1 && prev.orientation == 0) || (y == env->rows - 1 && prev.orientation == 1) || (x == 0 && prev.orientation == 2) || (y == 0 && prev.orientation == 3)) {
                    // std::cout<<"cannot forward: "<<(x==env->cols-1 && prev.orientation==0)<< (y==env->rows-1 && prev.orientation==1) << (x==0 && prev.orientation==2) << (y==0 )<<endl;
                    // std::cout<<"pos"<<x<<","<<y<<endl;
                    return State(-1, prev.timestep + 1, new_orientation = prev.orientation);
                }

                // if in the map but blocked
                if (env->map[new_location]) {
                    return State(-1, prev.timestep + 1, new_orientation = prev.orientation);
                }
            }
        } else if (action == ActionType::ROTATE) {
            new_orientation = (prev.orientation + 1) % 4;

        } else if (action == ActionType::COUNTER_ROTATE) {
            new_orientation = (prev.orientation - 1) % 4;
            if (new_orientation == -1)
                new_orientation = 3;
        }

        return State(new_location, prev.timestep + 1, new_orientation);
    }
};

#else
class CompetitionActionModelWithRotate {
public:
    list<std::tuple<std::string, int, int, int>> errors;

    CompetitionActionModelWithRotate(){};
    CompetitionActionModelWithRotate(Environment *env) : env(env), rows(env->rows), cols(env->cols) {
        moves[ActionType::R] = 1;
        moves[ActionType::D] = cols;
        moves[ActionType::L] = -1;
        moves[ActionType::U] = -cols;
        moves[ActionType::WAIT] = 0;
    };

    // bool is_valid(const std::vector<State>& prev, const std::vector<ActionType> & action);
    void set_logger(Logger *logger) { this->logger = logger; }

    std::vector<State> result_states(const std::vector<State> &prev, const std::vector<ActionType> &action) {
        std::vector<State> next(prev.size());
        for (size_t i = 0; i < prev.size(); i++) {
            next[i] = result_state(prev[i], action[i]);
        }
        return next;
    };

    std::vector<State> get_loc_neighbors(const State &curr, bool move_orient = true) {
        std::vector<State> neighbors;

        int x = curr.location % env->cols;
        int y = curr.location / env->cols;

        for (int i = 0; i < 4; ++i) {
            int new_location = curr.location + moves[i];
            int new_orientation;

            if (move_orient) {
                new_orientation = i;
            } else {
                new_orientation = 0;
            }

            if ((x == env->cols - 1 && i == 0) || (y == env->rows - 1 && i == 1) || (x == 0 && i == 2) || (y == 0 && i == 3)) {
                continue;
            }

            // if in the map but blocked
            if (env->map[new_location]) {
                continue;
            }

            neighbors.push_back(State(new_location, curr.timestep + 1, new_orientation));
        }

        return neighbors;
    }

    Environment *env = nullptr;
    int rows;
    int cols;
    int moves[5];
    Logger *logger = nullptr;

    State result_state(const State &prev, ActionType action) {
        // NOTE(rivers): we don't care about orientation, just keep it intact for now.

        int new_location = prev.location;
        int new_orientation = prev.orientation;

        if (action == ActionType::NA) {
            // std::cout<<"have action NA in result State"<<std::endl;
        } else {
            new_location = new_location + moves[action];
        }

        return State(new_location, prev.timestep + 1, new_orientation);
    }

    bool is_valid(const std::vector<State> &prev, const std::vector<ActionType> &actions) {
        if (prev.size() != actions.size()) {
            errors.push_back(make_tuple("incorrect std::vector size", -1, -1, prev[0].timestep + 1));
            return false;
        }

        std::vector<State> next = result_states(prev, actions);
        unordered_map<int, int> vertex_occupied;
        unordered_map<pair<int, int>, int> edge_occupied;

        for (int i = 0; i < prev.size(); i++) {

            if (next[i].location < 0 || next[i].location >= env->map.size() ||
                (abs(next[i].location / cols - prev[i].location / cols) + abs(next[i].location % cols - prev[i].location % cols) > 1)) {
                cout << "ERROR: agent " << i << " moves out of map size. " << endl;
                errors.push_back(make_tuple("unallowed move", i, -1, next[i].timestep));
                return false;
            }
            if (env->map[next[i].location] == 1) {
                cout << "ERROR: agent " << i << " moves to an obstacle. " << endl;
                errors.push_back(make_tuple("unallowed move", i, -1, next[i].timestep));
                return false;
            }

            if (vertex_occupied.find(next[i].location) != vertex_occupied.end()) {
                cout << "ERROR: agents " << i << " and " << vertex_occupied[next[i].location] << " have a vertex conflict. " << endl;
                errors.push_back(make_tuple("vertex conflict", i, vertex_occupied[next[i].location], next[i].timestep));
                return false;
            }

            int edge_idx = (prev[i].location + 1) * rows * cols + next[i].location;

            if (edge_occupied.find({prev[i].location, next[i].location}) != edge_occupied.end()) {
                cout << "ERROR: agents " << i << " and " << edge_occupied[{prev[i].location, next[i].location}] << " have an edge conflict. " << endl;
                errors.push_back(make_tuple("edge conflict", i, edge_occupied[{prev[i].location, next[i].location}], next[i].timestep));
                return false;
            }


            vertex_occupied[next[i].location] = i;
            int r_edge_idx = (next[i].location + 1) * rows * cols + prev[i].location;
            edge_occupied[{next[i].location, prev[i].location}] = i;
        }

        return true;
    }
};

#endif

#endif