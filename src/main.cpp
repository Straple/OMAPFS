#include <environment/info.hpp>
#include <environment/test_system.hpp>

#include <fstream>

int main() {
    get_planner_type() = PlannerType::EPIBT;
    get_scheduler_type() = SchedulerType::GREEDY;

    TestSystem test_system("tests/random/random-32-32-20.map", "tests/random/tasks.csv", "tests/random/agents_400.csv");
    Answer answer = test_system.simulate(1000);

    for (uint32_t action = 0; action <= ACTIONS_NUM; action++) {
        std::string filename = "heatmap_";
        if (action < ACTIONS_NUM) {
            filename += action_to_char(static_cast<ActionType>(action));
        } else {
            filename += "all";
        }
        filename += ".csv";
        std::ofstream output(filename);
        answer.write_heatmap(output, action);
    }
}
