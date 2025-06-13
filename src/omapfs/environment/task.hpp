#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>
#include <istream>

struct Task {
    uint32_t task_id = -1;
    uint32_t agent_assigned = -1;
    std::vector<uint32_t> targets;
};

class TaskPool {
    std::unordered_map<uint32_t, Task> pool;

    // for generate next task
    // [i] = sequence of targets for some task
    std::vector<std::vector<uint32_t>> task_targets;
    uint32_t next_task = 0;

public:

    Task &at(uint32_t task_id);

    void insert(const Task &task);

    [[nodiscard]] uint32_t size() const;

    std::unordered_map<uint32_t, Task>::iterator begin();

    std::unordered_map<uint32_t, Task>::iterator end();

    void gen_next_task();

    friend std::istream &operator>>(std::istream &input, TaskPool &pool);
};

TaskPool &get_task_pool();
