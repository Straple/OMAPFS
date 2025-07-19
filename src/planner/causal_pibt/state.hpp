#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <cstdint>
#include <vector>

namespace DefaultPlanner {

    struct State {
        int location = -1;
        int timestep = -1;
        int orientation = -1;// 0:east, 1:south, 2:west, 3:north

        struct Hasher {
            uint64_t operator()(const State &n) const;
        };

        void operator=(const State &other);

        bool operator==(const State &other) const;

        bool operator!=(const State &other) const;

        State() = default;

        State(int location, int timestep = -1, int orientation = -1);

        State(const State &other);
    };

    typedef std::vector<State> Path;

};// namespace DefaultPlanner
#endif