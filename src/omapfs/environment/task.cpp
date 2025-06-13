#include <omapfs/environment/task.hpp>

#include <omapfs/basic/assert.hpp>
#include <omapfs/basic/tools.hpp>

Task &TaskPool::at(uint32_t task_id) {
    ASSERT(pool.contains(task_id), "no contains");
    return pool.at(task_id);
}

void TaskPool::insert(const Task &task) {
    ASSERT(!pool.contains(task.task_id), "already contains");
    pool.emplace(task.task_id, task);
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

std::istream &operator>>(std::istream &input, TaskPool &pool) {
    ASSERT(input, "invalid input");
    std::string line;
    std::getline(input, line);
    ASSERT(line == "targets", "invalid header");
    while (std::getline(input, line)) {
        auto strs = split(line, ',');
        pool.task_targets.emplace_back();
        for (auto &s: strs) {
            pool.task_targets.back().push_back(std::stoll(s));
        }
    }
    return input;
}

TaskPool &get_task_pool() {
    static TaskPool task_pool;
    return task_pool;
}
