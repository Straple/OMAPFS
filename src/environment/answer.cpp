#include <environment/answer.hpp>

#include <basic/assert.hpp>
#include <environment/graph.hpp>

#include <map>

void Answer::validate_actions(uint32_t step) const {
    // проверить, что нет коллизий по вершинам и ребрам

    std::map<uint32_t, uint32_t> pos_usage, edge_usage;
    for (uint32_t r = 0; r < robots.size(); r++) {
        Position pos = robots[r].positions[step];
        ActionType action = robots[r].actions[step];
        Position to = pos.simulate(action);

        {
            uint32_t pos_id = to.get_pos();
            ASSERT(!pos_usage.contains(pos_id), "vertex collision");
            pos_usage[pos_id] = r;
        }

        if (
#ifdef ENABLE_ROTATE_MODEL
                action == ActionType::FORWARD
#else
                action != ActionType::WAIT
#endif
        ) {
            uint32_t edge_id = get_graph().get_to_edge(get_graph().get_node(pos), static_cast<uint32_t>(action));
            ASSERT(!edge_usage.contains(edge_id), "edge collision");
            edge_usage[edge_id] = r;
        }
    }
}

void Answer::validate_schedule(uint32_t step) const {
    // проверить, что нет коллизий по задачам

    // task_used[task_id] = assigned agent
    std::map<uint32_t, uint32_t> task_used;
    for (uint32_t r = 0; r < robots.size(); r++) {
        uint32_t task_id = robots[r].assigned_task[step];
        if (task_id != -1) {
            ASSERT(tasks[step].contains(task_id), "invalid task id");
            ASSERT(tasks[step].at(task_id).task_id == task_id, "invalid task id");

            if (tasks[step].at(task_id).is_taken) {
                // эта задача уже выполняется кем-то другим и не может быть переназначена
                ASSERT(tasks[step].at(task_id).agent_assigned == r, "invalid task assigned");
            }
            ASSERT(!task_used.contains(task_id), "task collision");
            task_used[task_id] = r;
        }
    }
}

void Answer::write_heatmap(std::ostream &output, uint32_t action) const {
    output << "row\\col";
    for (uint32_t col = 0; col < get_map().get_cols(); col++) {
        output << ',' << col;
    }
    output << '\n';
    for (uint32_t row = 0; row < get_map().get_rows(); row++) {
        output << row;
        for (uint32_t col = 0; col < get_map().get_cols(); col++) {
            output << ',' << heatmap[row][col][action];
        }
        output << '\n';
    }
}
