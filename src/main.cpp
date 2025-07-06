#include <omapfs/environment/info.hpp>
#include <omapfs/environment/test_system.hpp>

int main() {
    // EPIBT: 3459
    // EPIBT+LNS: 4500
    // PEPIBT+LNS: 5027
    get_planner_type() = PlannerType::PEPIBT_LNS;

    TestSystem test_system("tests/random/random-32-32-20.map", "tests/random/tasks.csv", "tests/random/agents_400.csv");
    test_system.simulate(1000);
}
