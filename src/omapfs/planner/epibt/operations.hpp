#pragma once

#include <omapfs/environment/action_model.hpp>

#include <array>
#include <cstdint>
#include <vector>
#include <iostream>

static constexpr inline uint32_t DEPTH = 5;

using Operation = std::array<ActionType, DEPTH>;

using Path = std::array<uint32_t, DEPTH>;

std::vector<Operation> &get_operations();

//uint32_t get_operation_depth(uint32_t index);

//std::vector<uint32_t> &get_operations_ids(uint32_t d);

void init_operations();

std::ostream &operator<<(std::ostream &output, const Operation &op);

std::istream &operator>>(std::istream &input, Operation &op);
