#include <environment/graph_guidance.hpp>

#include <environment/position.hpp>
#include <utils/assert.hpp>

constexpr uint32_t PENALTY_WEIGHT = 200;
constexpr uint32_t OK_WEIGHT = 20;

void GraphGuidance::set(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t dir, uint32_t action, uint32_t value) {
    for (int32_t x = x0; x <= x1; x++) {
        for (int32_t y = y0; y <= y1; y++) {
            int32_t pos = x * static_cast<int32_t>(cols) + y + 1;
            ASSERT(0 < pos && pos < graph.size(), "invalid pos");
            ASSERT(dir < 4, "invalid dir");
            ASSERT(action < 4, "invalid action");
#ifdef ENABLE_ROTATE_MODEL
            graph[pos][dir][action] = value;
#else
            graph[pos][dir] = value;
#endif
        }
    }
}

void GraphGuidance::add(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t dir, uint32_t action, uint32_t value) {
    for (int32_t x = x0; x <= x1; x++) {
        for (int32_t y = y0; y <= y1; y++) {
            int32_t pos = x * static_cast<int32_t>(cols) + y + 1;
            ASSERT(0 < pos && pos < graph.size(), "invalid pos");
            ASSERT(dir < 4, "invalid dir");
            ASSERT(action < 4, "invalid action");
#ifdef ENABLE_ROTATE_MODEL
            graph[pos][dir][action] += value;
#else
            graph[pos][dir] += value;
#endif
        }
    }
}

void GraphGuidance::set_default() {
#ifdef ENABLE_ROTATE_MODEL
    for (uint32_t pos = 1; pos < graph.size(); pos++) {
        for (uint32_t dir = 0; dir < DIRECTIONS_NUM; dir++) {
            for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
                graph[pos][dir][action] = 1;
            }
        }
    }
#else
    for (uint32_t pos = 1; pos < graph.size(); pos++) {
        for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
            graph[pos][action] = 1;
        }
    }
#endif
}

void GraphGuidance::set_grid() {
    for (uint32_t x = 0; x < rows; x++) {
        if (x & 1) {
            set(x, 0, x, cols - 1, 0, 0, PENALTY_WEIGHT);
            set(x, 0, x, cols - 1, 2, 0, OK_WEIGHT);
        } else {
            set(x, 0, x, cols - 1, 2, 0, PENALTY_WEIGHT);
            set(x, 0, x, cols - 1, 0, 0, OK_WEIGHT);
        }
    }
    for (uint32_t y = 0; y < cols; y++) {
        if (y & 1) {
            set(0, y, rows - 1, y, 1, 0, PENALTY_WEIGHT);
            set(0, y, rows - 1, y, 3, 0, OK_WEIGHT);
        } else {
            set(0, y, rows - 1, y, 3, 0, PENALTY_WEIGHT);
            set(0, y, rows - 1, y, 1, 0, OK_WEIGHT);
        }
    }
}

void GraphGuidance::set_warehouse() {
    set_grid();

    uint32_t msk = 0b10100101;
    int bit = 0;
    for (uint32_t y = 0; y < cols; y++) {
        if ((msk >> bit) & 1) {
            set(0, y, rows - 1, y, 1, 0, PENALTY_WEIGHT);
            set(0, y, rows - 1, y, 3, 0, OK_WEIGHT);
        } else {
            set(0, y, rows - 1, y, 3, 0, PENALTY_WEIGHT);
            set(0, y, rows - 1, y, 1, 0, OK_WEIGHT);
        }
        bit = (bit + 1) % 8;
    }

    // [KEK]: повышает вес верхней и нижней плашки, что уменьшает загруженность агентов там
    // реально улучшает
    // 36754 -> 37548
    {
        add(3, 0, 10, cols - 1, 0, 0, 1);
        add(3, 0, 10, cols - 1, 1, 0, 1);
        add(3, 0, 10, cols - 1, 2, 0, 1);
        add(3, 0, 10, cols - 1, 3, 0, 1);

        add(125, 0, 137, cols - 1, 0, 0, 1);
        add(125, 0, 137, cols - 1, 1, 0, 1);
        add(125, 0, 137, cols - 1, 2, 0, 1);
        add(125, 0, 137, cols - 1, 3, 0, 1);
    }
}

void GraphGuidance::set_sortation() {
    {
        uint32_t msk = 0b1001;
        int bit = 0;
        for (uint32_t x = 0; x < rows; x++) {
            if ((msk >> bit) & 1) {
                set(x, 0, x, cols - 1, 0, 0, PENALTY_WEIGHT);
                set(x, 0, x, cols - 1, 2, 0, OK_WEIGHT);
            } else {
                set(x, 0, x, cols - 1, 2, 0, PENALTY_WEIGHT);
                set(x, 0, x, cols - 1, 0, 0, OK_WEIGHT);
            }
            bit = (bit + 1) % 4;
        }
    }

    {
        uint32_t msk = 0b1100;
        int bit = 0;
        for (uint32_t y = 0; y < cols; y++) {
            if ((msk >> bit) & 1) {
                set(0, y, rows - 1, y, 1, 0, PENALTY_WEIGHT);
                set(0, y, rows - 1, y, 3, 0, OK_WEIGHT);
            } else {
                set(0, y, rows - 1, y, 3, 0, PENALTY_WEIGHT);
                set(0, y, rows - 1, y, 1, 0, OK_WEIGHT);
            }
            bit = (bit + 1) % 4;
        }
    }

    // [KEK]: повышает вес верхней и нижней плашки, что уменьшает загруженность агентов там
    // реально улучшает
    // 35816 -> 38860
    {
        add(0, 0, 8, cols - 1, 0, 0, 1);
        add(0, 0, 8, cols - 1, 1, 0, 1);
        add(0, 0, 8, cols - 1, 2, 0, 1);
        add(0, 0, 8, cols - 1, 3, 0, 1);

        add(130, 0, 139, cols - 1, 0, 0, 1);
        add(130, 0, 139, cols - 1, 1, 0, 1);
        add(130, 0, 139, cols - 1, 2, 0, 1);
        add(130, 0, 139, cols - 1, 3, 0, 1);
    }
}

void GraphGuidance::set_game() {
    set_grid();
}

void GraphGuidance::set_city() {
    set_grid();
}

void GraphGuidance::set_walls() {
    for (uint32_t x = 0; x < rows; x++) {
        for (uint32_t y = 0; y < cols; y++) {
            uint32_t pos = x * cols + y + 1;
            for (uint32_t dir = 0; dir < 4; dir++) {
                for (uint32_t action = 0; action < 4; action++) {
#ifdef ENABLE_ROTATE_MODEL
                    if (!Position(x, y, dir).is_valid()) {
                        graph[pos][dir][action] = 0;
                    }
#else
                    if (!Position(x, y).is_valid()) {
                        graph[pos][action] = 0;
                    }
#endif
                }
            }
        }
    }
}

GraphGuidance::GraphGuidance(uint32_t rows, uint32_t cols) : rows(rows), cols(cols), graph(rows * cols + 1) {
    set_default();
}

GraphGuidance::GraphGuidance(const GuidanceMap &gmap)
    : rows(gmap.get_rows()), cols(gmap.get_cols()), graph(gmap.get_rows() * gmap.get_cols() + 1) {
    set_default();


    for (uint32_t x = 0; x < rows; x++) {
        for (uint32_t y = 0; y < cols; y++) {
            if (gmap.get(x, y) == '@') {
                continue;
            }
            uint32_t pos = x * cols + y + 1;

            constexpr uint32_t w1 = 2;
            constexpr uint32_t w2 = 4;
            constexpr uint32_t w3 = 6;

            for (uint32_t dir = 0; dir < 4; dir++) {
                for (uint32_t action = 0; action < ACTIONS_NUM; action++) {
#ifdef ENABLE_ROTATE_MODEL
                    graph[pos][dir][action] = w1;
#else
                    graph[pos][action] = w1;
#endif
                }
            }

            uint32_t dir = 0;
            if (gmap.get(x, y) == '>') {
                dir = 0;
            } else if (gmap.get(x, y) == 'v') {
                dir = 1;
            } else if (gmap.get(x, y) == '<') {
                dir = 2;
            } else if (gmap.get(x, y) == '^') {
                dir = 3;
            } else {// '.'
                continue;
            }

            // 0:east  >
            // 1:south v
            // 2:west  <
            // 3:north ^

#ifdef ENABLE_ROTATE_MODEL
            //FW:  0
            //CR:  1
            //CCR: 2
            //W:   3

            // смотрит в нужное направление
            // >
            graph[pos][dir][0] = w1;// FW
            graph[pos][dir][1] = w1;// CR
            graph[pos][dir][2] = w1;// CCR
            graph[pos][dir][3] = w1;// W

            dir = (dir + 1) % 4;

            // v
            graph[pos][dir][0] = w3;// FW
            graph[pos][dir][1] = w1;// CR
            graph[pos][dir][2] = w1;// CCR
            graph[pos][dir][3] = w1;// W

            dir = (dir + 1) % 4;

            // <
            graph[pos][dir][0] = w3;// FW
            graph[pos][dir][1] = w1;// CR
            graph[pos][dir][2] = w1;// CCR
            graph[pos][dir][3] = w1;// W

            dir = (dir + 1) % 4;

            // ^
            graph[pos][dir][0] = w3;// FW
            graph[pos][dir][1] = w1;// CR
            graph[pos][dir][2] = w1;// CCR
            graph[pos][dir][3] = w1;// W

#else
            // смотрит в нужное направление
            // >
            graph[pos][dir] = w1;
            graph[pos][(dir + 1) % 4] = w2;
            graph[pos][(dir + 3) % 4] = w2;
            graph[pos][(dir + 2) % 4] = w3;
#endif
        }
    }

    set_walls();

    /*{
        std::ofstream output("graph_guidance");
        output << *this;
    }
    _exit(0);*/
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
