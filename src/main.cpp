#include <omapfs/environment/test_system.hpp>

int main() {
    TestSystem test_system("tests/random/random-32-32-20.map", "tests/random/tasks.csv", "tests/random/agents_400.csv");
    test_system.simulate(1000);
}
