#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <iostream>
#include <vector>

namespace DefaultPlanner {
    struct s_node {
        int label = 0;
        int id = -1;//also location, -1 indicated not generated yet.
        int g = 0;
        int h = 0;
        int op_flow = 0;
        int all_vertex_flow = 0;
        bool closed = false;
        int depth = 0;
        double tie_breaker = 0;
        s_node *parent = nullptr;

        unsigned int priority = 0;

        s_node(int id, int g, int h, int op_flow, int depth);

        s_node() = default;

        int get_f() const;

        bool is_closed() const;

        void close();

        int get_op_flow() const;

        int get_all_vertex_flow() const;

        void set_all_flow(int op_flow, int all_vertex_flow);

        int get_g() const;

        int get_h() const;

        unsigned int get_priority() const;

        void set_priority(unsigned int p);

        void reset();
    };

    struct equal_search_node {
        bool operator()(const s_node &lhs, const s_node &rhs) const;
    };


    struct re_f {
        bool operator()(const s_node &lhs, const s_node &rhs) const;
    };

    struct re_jam {
        bool operator()(const s_node &lhs, const s_node &rhs) const;
    };

    struct cmp_less_f//focal search open
    {
        bool operator()(const s_node &lhs, const s_node &rhs) const;
    };

    struct re_of {//re-expansion rule astar
        bool operator()(const s_node &lhs, const s_node &rhs) const;
    };

    struct cmp_less_of//astar open
    {
        bool operator()(const s_node &lhs, const s_node &rhs) const;
    };

}// namespace DefaultPlanner
#endif