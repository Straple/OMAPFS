#pragma once

#include <environment/action_model.hpp>
#include <environment/position.hpp>
#include <environment/task.hpp>

#include <map>

struct Answer {
    struct Robot {
        // initial position
        std::vector<Position> positions;
        // [step] = action in this step
        std::vector<ActionType> actions;
        // [step] = assigned task in this step
        std::vector<uint32_t> assigned_task;
    };

    uint32_t steps_num = 0;

    std::vector<Robot> robots;

    // [step] = task pool
    std::vector<std::map<uint32_t, Task>> tasks;

    // [row][col][action]
    std::vector<std::vector<std::array<uint64_t, ACTIONS_NUM + 1>>> heatmap;

    void validate_actions(uint32_t step) const;

    void validate_schedule(uint32_t step) const;

    void write_heatmap(std::ostream& output, uint32_t action) const;
};
