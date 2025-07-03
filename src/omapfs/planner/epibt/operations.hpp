#pragma once

#include <omapfs/environment/action_model.hpp>

#include <array>
#include <cstdint>
#include <vector>
#include <iostream>

static constexpr inline uint32_t DEPTH = 3;

using Operation = std::array<ActionType, DEPTH>;

using Path = std::array<uint32_t, DEPTH>;

std::vector<Operation> &get_operations();

void init_operations();

std::ostream &operator<<(std::ostream &output, const Operation &op);

std::istream &operator>>(std::istream &input, Operation &op);
