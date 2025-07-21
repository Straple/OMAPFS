#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <iostream>
#include <vector>

#include <planner/causal_pibt/search_node.hpp>

namespace CausalPlanner {

    class MemoryPool {
        //every location in the map has a node with id equal to the location
    public:
        MemoryPool();
        MemoryPool(int size);

        int generated();

        void init(int size);

        bool is_ready();

        bool has_node(int id);

        bool is_closed(int id);

        s_node *get_node(int id);

        void close_node(int id);

        s_node *generate_node(int id, int g, int h, int op_flow, int depth, int all_vertex_flow = 0);

        void free_node(int id);

        void reset();

        ~MemoryPool();

    private:
        s_node *nodes;
        int size;
        int index;
        int label;
        bool ready = false;
    };

}// namespace CausalPlanner
#endif
