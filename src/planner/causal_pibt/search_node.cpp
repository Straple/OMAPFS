#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/search_node.hpp>

using namespace DefaultPlanner;

s_node::s_node(int id, int g, int h, int op_flow, int depth) : id(id), g(g), h(h), op_flow(op_flow), depth(depth) {}

int s_node::get_f() const {
    return g + h;
}

bool s_node::is_closed() const {
    return closed;
}

void s_node::close() {
    closed = true;
}

int s_node::get_op_flow() const {
    return op_flow;
}

int s_node::get_all_vertex_flow() const {
    return all_vertex_flow;
}

void s_node::set_all_flow(int op_flow, int all_vertex_flow) {
    this->op_flow = op_flow;
    this->all_vertex_flow = all_vertex_flow;
}

int s_node::get_g() const {
    return g;
}

int s_node::get_h() const {
    return h;
}

unsigned int s_node::get_priority() const {
    return priority;
}

void s_node::set_priority(unsigned int p) {
    priority = p;
}

void s_node::reset() {
    label = 0;
    id = -1;
    g = 0;
    h = 0;
    op_flow = 0;
    all_vertex_flow = 0;
    closed = false;
    depth = 0;
    parent = nullptr;
    tie_breaker = 0;
}

bool equal_search_node::operator()(const s_node &lhs, const s_node &rhs) const {
    return lhs.id == rhs.id && lhs.get_op_flow() == rhs.get_op_flow() && lhs.get_all_vertex_flow() == rhs.get_all_vertex_flow() && lhs.get_g() == rhs.get_g();
}

bool re_f::operator()(const s_node &lhs, const s_node &rhs) const {
    return lhs.get_f() < rhs.get_f();
}

bool re_jam::operator()(const s_node &lhs, const s_node &rhs) const {
    if (lhs.tie_breaker == rhs.tie_breaker) {
        return lhs.get_f() < rhs.get_f();
    } else
        return lhs.tie_breaker < rhs.tie_breaker;
}

bool cmp_less_f::operator()(const s_node &lhs, const s_node &rhs) const {
    if (lhs.get_f() == rhs.get_f()) {
        if (lhs.get_g() == rhs.get_g())
            return false;// rand() % 2;
        else
            return lhs.get_g() > rhs.get_g();
    } else
        return lhs.get_f() < rhs.get_f();
}

bool re_of::operator()(const s_node &lhs, const s_node &rhs) const {
    return lhs.get_op_flow() + lhs.get_all_vertex_flow() + lhs.get_f() < rhs.get_op_flow() + rhs.get_all_vertex_flow() + rhs.get_f();
}

bool cmp_less_of::operator()(const s_node &lhs, const s_node &rhs) const {

    if (lhs.get_op_flow() + lhs.get_all_vertex_flow() + lhs.get_f() == rhs.get_op_flow() + rhs.get_all_vertex_flow() + rhs.get_f()) {
        if (lhs.get_f() == rhs.get_f()) {
            if (lhs.get_g() == rhs.get_g())
                if (lhs.tie_breaker == rhs.tie_breaker)
                    return false;// rand() % 2;
                else
                    return lhs.tie_breaker < rhs.tie_breaker;
            else
                return lhs.get_g() > rhs.get_g();
        } else
            return lhs.get_f() < rhs.get_f();
    } else
        return lhs.get_op_flow() + lhs.get_all_vertex_flow() + lhs.get_f() < rhs.get_op_flow() + rhs.get_all_vertex_flow() + rhs.get_f();
}

#endif
