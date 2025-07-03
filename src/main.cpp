#include <omapfs/environment/test_system.hpp>

int main() {
    TestSystem test_system("random-32-32-20.map", "tasks.csv", "agents.csv");
    test_system.simulate(100);
}
