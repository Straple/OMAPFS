#pragma once

#include <environment/action_model.hpp>

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

// Use CMake-defined EPIBT_DEPTH, with fallback default
#ifndef EPIBT_DEPTH
#define EPIBT_DEPTH 3
#endif

// Convert macro to constexpr
constexpr uint32_t EPIBT_DEPTH_VALUE = EPIBT_DEPTH;

using Operation = std::array<ActionType, EPIBT_DEPTH_VALUE>;

using Path = std::array<uint32_t, EPIBT_DEPTH_VALUE>;

std::vector<Operation> &get_operations();

uint32_t get_operation_depth(uint32_t index);

// FFF -> FFW
uint32_t get_operation_next(uint32_t index);

std::vector<uint32_t> &get_operations_ids(uint32_t d);

uint32_t get_operation_weight(uint32_t index);

void init_operations();

std::ostream &operator<<(std::ostream &output, const Operation &op);

std::istream &operator>>(std::istream &input, Operation &op);
