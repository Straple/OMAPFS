#include <environment/task.hpp>

#include <utils/assert.hpp>
#include <utils/tools.hpp>
#include <environment/robot.hpp>

bool TaskPool::contains(uint32_t task_id) const {
    return pool.contains(task_id);
}

Task &TaskPool::at(uint32_t task_id) {
    ASSERT(pool.contains(task_id), "no contains");
    return pool.at(task_id);
}

void TaskPool::insert(const Task &task) {
    ASSERT(!pool.contains(task.task_id), "already contains");
    pool.emplace(task.task_id, task);
}

void TaskPool::erase(uint32_t task_id) {
    ASSERT(pool.contains(task_id), "no contains");
    pool.erase(task_id);
}

[[nodiscard]] uint32_t TaskPool::size() const {
    return pool.size();
}

std::unordered_map<uint32_t, Task>::iterator TaskPool::begin() {
    return pool.begin();
}

std::unordered_map<uint32_t, Task>::iterator TaskPool::end() {
    return pool.end();
}

void TaskPool::gen_next_task() {
    insert(Task{next_task, static_cast<uint32_t>(-1), task_targets[next_task % task_targets.size()]});
    next_task++;
}

uint32_t TaskPool::gen_const_next_task(uint32_t r) {
    if (task_counter.size() <= r) {
        task_counter.resize(get_robots().size());
    }
    Task task{next_task, static_cast<uint32_t>(-1), task_targets[(task_counter[r] * get_robots().size()) % task_targets.size()]};
    task.targets.resize(1);
    task_counter[r]++;
    next_task++;
    insert(task);
    return task.task_id;
}

std::istream &operator>>(std::istream &input, TaskPool &pool) {
    ASSERT(input, "invalid input");
    std::string line;
    std::getline(input, line);
    ASSERT(line == "targets", "invalid header");
    while (std::getline(input, line)) {
        auto strs = split(line, ',');
        pool.task_targets.emplace_back();
        for (auto &s: strs) {
            pool.task_targets.back().push_back(std::stoll(s) + 1);
        }
    }
    return input;
}

TaskPool &get_task_pool() {
    static TaskPool task_pool;
    return task_pool;
}
