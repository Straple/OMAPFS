#pragma once

#include <cstdint>
#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

struct Task {
    uint32_t task_id = -1;
    uint32_t agent_assigned = -1;
    std::vector<uint32_t> targets;
    bool is_taken = false;
};

class TaskPool {
    std::unordered_map<uint32_t, Task> pool;

    // for generate next task
    // [i] = sequence of targets for some task
    std::vector<std::vector<uint32_t>> task_targets;
    uint32_t next_task = 0;

    // [r] = number of finished tasks
    std::vector<uint32_t> task_counter;

public:
    [[nodiscard]] bool contains(uint32_t task_id) const;

    Task &at(uint32_t task_id);

    void insert(const Task &task);

    void erase(uint32_t task_id);

    [[nodiscard]] uint32_t size() const;

    std::unordered_map<uint32_t, Task>::iterator begin();

    std::unordered_map<uint32_t, Task>::iterator end();

    void gen_next_task();

    uint32_t gen_const_next_task(uint32_t r);

    friend std::istream &operator>>(std::istream &input, TaskPool &pool);
};

TaskPool &get_task_pool();
