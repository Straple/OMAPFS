#include <environment/graph_guidance.hpp>

#include <basic/assert.hpp>
#include <environment/position.hpp>

GraphGuidance::GraphGuidance(uint32_t rows, uint32_t cols) : rows(rows), cols(cols), graph(rows * cols + 1) {
#ifdef ENABLE_ROTATE_MODEL
    for (uint32_t dir = 0; dir < DIRECTIONS_NUM; dir++) {
        for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
            for (uint32_t pos = 1; pos < graph.size(); pos++) {
                graph[pos][dir][action] = 1;
            }
        }
    }
#else
    for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
        for (uint32_t pos = 1; pos < graph.size(); pos++) {
            graph[pos][action] = 1;
        }
    }
#endif
}

uint32_t GraphGuidance::get(const Position &p, uint32_t action) const {
    ASSERT(p.get_pos() < graph.size(), "invalid pos");
    ASSERT(action < ACTIONS_NUM, "invalid action");
#ifdef ENABLE_ROTATE_MODEL
    ASSERT(p.get_dir() < DIRECTIONS_NUM, "invalid dir");
    return graph[p.get_pos()][p.get_dir()][action];
#else
    return graph[p.get_pos()][action];
#endif
}

void GraphGuidance::set(const Position &p, uint32_t action, uint32_t weight) {
    ASSERT(p.get_pos() < graph.size(), "invalid pos");
    ASSERT(action < ACTIONS_NUM, "invalid action");
#ifdef ENABLE_ROTATE_MODEL
    ASSERT(p.get_dir() < DIRECTIONS_NUM, "invalid dir");
    graph[p.get_pos()][p.get_dir()][action] = weight;
#else
    graph[p.get_pos()][action] = weight;
#endif
}

uint32_t GraphGuidance::get_size() const {
    return graph.size();
}

uint32_t GraphGuidance::get_rows() const {
    return rows;
}

uint32_t GraphGuidance::get_cols() const {
    return cols;
}

std::istream &operator>>(std::istream &input, GraphGuidance &gg) {
    uint32_t rows, cols;
    input >> rows >> cols;
    gg = GraphGuidance(rows, cols);
#ifdef ENABLE_ROTATE_MODEL
    for (uint32_t dir = 0; dir < DIRECTIONS_NUM; dir++) {
        for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
            for (uint32_t pos = 1; pos < gg.graph.size(); pos++) {
                input >> gg.graph[pos][dir][action];
            }
        }
    }
#else
    for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
        for (uint32_t pos = 1; pos < gg.graph.size(); pos++) {
            input >> gg.graph[pos][action];
        }
    }
#endif
    return input;
}

std::ostream &operator<<(std::ostream &output, const GraphGuidance &gg) {
    output << gg.rows << ' ' << gg.cols << '\n';
#ifdef ENABLE_ROTATE_MODEL
    for (uint32_t dir = 0; dir < DIRECTIONS_NUM; dir++) {
        for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
            for (uint32_t pos = 1; pos < gg.graph.size(); pos++) {
                if (pos != 1) {
                    output << ' ';
                }
                output << gg.graph[pos][dir][action];
            }
            output << '\n';
        }
    }
#else
    for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
        for (uint32_t pos = 1; pos < gg.graph.size(); pos++) {
            if (pos != 1) {
                output << ' ';
            }
            output << gg.graph[pos][action];
        }
        output << '\n';
    }
#endif
    return output;
}

GraphGuidance &get_gg() {
    static GraphGuidance gg;
    return gg;
}
