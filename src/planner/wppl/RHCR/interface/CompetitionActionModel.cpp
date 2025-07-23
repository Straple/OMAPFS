#include <planner/wppl/util/CompetitionActionModel.h>
#ifdef ENABLE_ROTATE_MODEL

#include <iostream>
#include <map>
#include <unordered_map>

bool CompetitionActionModelWithRotate::is_valid(const std::vector<State> &prev, const std::vector<ActionType> &actions) {
    if (prev.size() != actions.size()) {
        errors.push_back(std::make_tuple("incorrect std::vector size", -1, -1, prev[0].timestep + 1));
        return false;
    }

    std::vector<State> next = result_states(prev, actions);
    std::unordered_map<int, int> vertex_occupied;
    std::map<std::pair<int, int>, int> edge_occupied;

    for (int i = 0; i < prev.size(); i++) {

        if (next[i].location < 0 or next[i].location >= env->map.size()) {
            std::cout << "ERROR: agent " << i << "up or down out-of-boundary." << std::endl;
            errors.push_back(std::make_tuple("up/down OOB", i, -1, next[i].timestep));
            return false;
        }

        if (prev[i].location == next[i].location) {
            // check if the rotation is not larger than 90 degree
            if (abs(prev[i].orientation - next[i].orientation) == 2) {
                std::cout << "ERROR: agent " << i << " over-rotates. " << std::endl;
                errors.push_back(std::make_tuple("over-rotate", i, -1, next[i].timestep));
                return false;
            }
        } else {
            if (prev[i].orientation != next[i].orientation) {
                std::cout << "ERROR: agent " << i << " moves and rotates at the same time. " << std::endl;
                errors.push_back(std::make_tuple("unallowed move", i, -1, next[i].timestep));
                return false;
            }
            if (next[i].location - prev[i].location != moves[prev[i].orientation]) {
                std::cout << "ERROR: agent " << i << " moves in a wrong direction. " << std::endl;
                errors.push_back(std::make_tuple("unallowed move", i, -1, next[i].timestep));
                return false;
            }
            if (abs(next[i].location / cols - prev[i].location / cols) + abs(next[i].location % cols - prev[i].location % cols) > 1) {
                std::cout << "ERROR: agent " << i << " moves more than 1 steps. or left or right out-of-boundary" << std::endl;
                errors.push_back(std::make_tuple("unallowed move or left/right OOB", i, -1, next[i].timestep));
                return false;
            }
        }

        if (env->map[next[i].location] == 1) {
            std::cout << "ERROR: agent " << i << " moves to an obstacle. state:"
                      << "???" /*prev[i]*/ << " action:"
                      << "???" /*actions[i]*/ << std::endl;
            errors.push_back(std::make_tuple("unallowed move", i, -1, next[i].timestep));
            return false;
        }


        if (vertex_occupied.find(next[i].location) != vertex_occupied.end()) {
            std::cout << "ERROR: agents " << i << " and " << vertex_occupied[next[i].location] << " have a vertex conflict. " << std::endl;
            errors.push_back(std::make_tuple("vertex conflict", i, vertex_occupied[next[i].location], next[i].timestep));
            return false;
        }

        int edge_idx = (prev[i].location + 1) * rows * cols + next[i].location;

        if (edge_occupied.find({prev[i].location, next[i].location}) != edge_occupied.end()) {
            std::cout << "ERROR: agents " << i << " and " << edge_occupied[{prev[i].location, next[i].location}] << " have an edge conflict. " << std::endl;
            errors.push_back(std::make_tuple("edge conflict", i, edge_occupied[{prev[i].location, next[i].location}], next[i].timestep));
            return false;
        }


        vertex_occupied[next[i].location] = i;
        int r_edge_idx = (next[i].location + 1) * rows * cols + prev[i].location;
        edge_occupied[{next[i].location, prev[i].location}] = i;
    }

    return true;
}
#endif
