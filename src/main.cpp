#include <omapfs/environment/map.hpp>
#include <omapfs/environment/graph.hpp>
#include <omapfs/environment/graph_guidance.hpp>
#include <omapfs/environment/heuristic_matrix.hpp>
#include <omapfs/planner/epibt/operations.hpp>

#include <fstream>

int main() {
    std::ifstream input("random-32-32-20.map");
    input >> get_map();

    // environment
    get_gg() = GraphGuidance(get_map().get_rows(), get_map().get_cols());
    get_graph() = Graph(get_map(), get_gg());
    get_hm() = HeuristicMatrix(get_graph());

    // epibt
    init_operations();
}
