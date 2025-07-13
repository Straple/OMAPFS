#pragma once

#include <planner/epibt/epibt.hpp>
#include <utils/randomizer.hpp>

// Enhanced Priority Inheritance with BackTracking + Large Neighborhood Search
class EPIBT_LNS : public EPIBT {

    double temp = 1;

    uint32_t lns_step = 0;

    Randomizer rnd;

    bool consider();

    RetType try_build(uint32_t r, uint32_t &counter);

    void try_build(uint32_t r);

public:
    EPIBT_LNS(Robots robots, TimePoint end_time, const std::vector<uint32_t> &operations);

    void solve(uint64_t seed);

    void parallel_solve(uint64_t seed);

    [[nodiscard]] uint32_t get_lns_steps() const;
};
