#pragma once

#include <utils/time.hpp>

#include <settings.hpp>

#include <cstdint>
#include <vector>

class GreedyScheduler {

    double cur_score = 0;

    // desires[r] = task id
    std::vector<uint32_t> desires;

    // task_to_robot[task] = robot id
    std::vector<uint32_t> task_to_robot;

    std::vector<uint32_t> free_robots;

    std::vector<uint32_t> free_tasks;

    // dp[r] = отсортированный вектор (dist, task_id)
    std::vector<std::vector<std::pair<uint32_t, uint32_t>>> dp;

    std::vector<uint32_t> timestep_updated;

    // task_metric[t]
    std::vector<uint32_t> task_metric;

    // task_target[t] = цель задачи (pos)
    std::vector<uint32_t> task_target;

    void rebuild_dp(uint32_t r);

    [[nodiscard]] uint64_t get_dist(uint32_t r, uint32_t t) const;

    void remove(uint32_t r);

    void add(uint32_t r, uint32_t t);

    void validate();

public:
    GreedyScheduler() = default;

    void update();

    void rebuild_dp(TimePoint end_time);

    void solve(TimePoint end_time);

    [[nodiscard]] std::vector<uint32_t> get_schedule() const;

    [[nodiscard]] double get_score() const;
};
