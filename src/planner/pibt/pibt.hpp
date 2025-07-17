#pragma once

#include <environment/robot.hpp>
#include <planner/epibt/operations.hpp>
#include <utils/time.hpp>

class PIBT {
    Robots &robots;

    std::vector<DirectionType> desires;

    std::vector<uint32_t> visited;

    std::vector<uint32_t> order;

    uint32_t visited_counter = 0;

    TimePoint end_time;

#ifdef ENABLE_ROTATE_MODEL
#define PIBT_PLAN_DEPTH 3
#else
#define PIBT_PLAN_DEPTH 1
#endif

    // used_edge[edge][depth] = robot id
    std::vector<std::array<uint32_t, PIBT_PLAN_DEPTH>> used_edge;

    // used_pos[pos][depth] = robot id
    std::vector<std::array<uint32_t, PIBT_PLAN_DEPTH>> used_pos;

    [[nodiscard]] std::array<uint32_t, PIBT_PLAN_DEPTH> get_nodes_path(uint32_t r, DirectionType desired) const;

    [[nodiscard]] std::array<uint32_t, PIBT_PLAN_DEPTH> get_poses_path(uint32_t r, DirectionType desired) const;

    [[nodiscard]] std::array<uint32_t, PIBT_PLAN_DEPTH> get_edges_path(uint32_t r, DirectionType desired) const;

    void add_path(uint32_t r);

    void remove_path(uint32_t r);

    [[nodiscard]] uint32_t get_used(uint32_t r, DirectionType desired) const;

    enum class RetType {
        FAILED,
        ACCEPTED,// success + accepted
        REJECTED,// success + not accepted
    };

    RetType build(uint32_t r, uint32_t priority, uint32_t &counter);

public:
    PIBT(Robots &robots, TimePoint end_time);

    void solve();

    [[nodiscard]] std::vector<DirectionType> get_desires() const;

    [[nodiscard]] std::vector<ActionType> get_actions() const;
};
