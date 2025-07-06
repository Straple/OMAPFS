#pragma once

#include <environment/position.hpp>

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

class GraphGuidance {
#ifdef ENABLE_ROTATE_MODEL
    // weight[pos][dir][action]
    std::vector<std::array<std::array<uint32_t, DIRECTIONS_NUM>, ACTIONS_NUM>> graph;
#else
    // weight[pos][action]
    std::vector<std::array<uint32_t, ACTIONS_NUM>> graph;
#endif

    uint32_t rows = 0;
    uint32_t cols = 0;

public:

    GraphGuidance() = default;

    GraphGuidance(uint32_t rows, uint32_t cols);

    [[nodiscard]] uint32_t get(const Position &p, uint32_t action) const;

    void set(const Position &p, uint32_t action, uint32_t weight);

    [[nodiscard]] uint32_t get_size() const;

    [[nodiscard]] uint32_t get_rows() const;

    [[nodiscard]] uint32_t get_cols() const;

    friend std::istream &operator>>(std::istream &input, GraphGuidance &gg);

    friend std::ostream &operator<<(std::ostream &output, const GraphGuidance &gg);
};

GraphGuidance &get_gg();
