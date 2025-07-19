#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/types.hpp>

using namespace DefaultPlanner;

PIBT_C::PIBT_C(int location, int heuristic, int orientation, int tie_breaker)
    : location(location), heuristic(heuristic), orientation(orientation), tie_breaker(tie_breaker) {
}

unsigned int HNode::get_priority() const {
    return priority;
}

void HNode::set_priority(unsigned int p) {
    priority = p;
}

HNode::HNode(int location, int direction, int value)
    : location(location), direction(direction), value(value) {
}

bool HNode::compare_node::operator()(const HNode &n1, const HNode &n2) const {
    return n1.value < n2.value;
}

bool HeuristicTable::empty() {
    return htable.empty();
}

d2p::d2p(int label, int id, int cost, int togo)
    : label(label), id(id), cost(cost), togo(togo) {
}

bool Dist2Path::empty() {
    return dist2path.empty();
}

#endif
