#ifdef ENABLE_ROTATE_MODEL

#include <planner/causal_pibt/memory.hpp>

#include <utils/assert.hpp>

using namespace CausalPlanner;

MemoryPool::MemoryPool() {
    size = 0;
    index = 0;
    nodes = nullptr;
    label = 0;
}

MemoryPool::MemoryPool(int size) {
    init(size);
}

int MemoryPool::generated() {
    return index;
}

void MemoryPool::init(int size) {
    this->size = size;
    index = 0;
    label = 0;
    nodes = new s_node[size];
    ready = true;
}

bool MemoryPool::is_ready() {
    return ready;
}

bool MemoryPool::has_node(int id) {
    if (id >= size) {
        std::cout << "range out of memory pool size " << id << "," << index << "," << size << std::endl;
        exit(1);
    }
    return nodes[id].label == label && nodes[id].id == id;
}

bool MemoryPool::is_closed(int id) {
    if (id >= size) {
        std::cout << "range out of memory pool size " << id << "," << index << "," << size << std::endl;
        exit(1);
    }
    if (nodes[id].label != label) {
        return false;
    }
    return nodes[id].is_closed();
}

s_node *MemoryPool::get_node(int id) {
    if (id >= size) {
        std::cout << "range out of memory pool size " << id << "," << index << "," << size << std::endl;
        exit(1);
    }
    if (nodes[id].label != label || nodes[id].id == -1) {
        std::cout << "error node not generated yet" << std::endl;
        exit(1);
    }
    return &(nodes[id]);
}

void MemoryPool::close_node(int id) {
    if (id >= size) {
        std::cout << "range out of memory pool size " << id << "," << index << "," << size << std::endl;
        exit(1);
    }
    if (nodes[id].label != label || nodes[id].id == -1) {
        std::cout << "node not generated yet" << std::endl;
        exit(1);
    }
    nodes[id].close();
}

s_node *MemoryPool::generate_node(int id, int g, int h, int op_flow, int depth, int all_vertex_flow) {
    if (id >= size) {
        std::cout << "range out of memory pool size " << id << "," << index << "," << size << std::endl;
        exit(1);
    }

    if (nodes[id].label == label && nodes[id].id != -1) {
        std::cout << "node already generated " << id << "," << is_ready() << std::endl;

        std::cout << "node already generated " << nodes[id].id << std::endl;
        exit(1);
    }
    nodes[id].reset();
    nodes[id].label = label;
    nodes[id].id = id;
    nodes[id].g = g;
    nodes[id].h = h;
    nodes[id].op_flow = op_flow;
    nodes[id].depth = depth;
    nodes[id].all_vertex_flow = all_vertex_flow;
    index++;
    return &(nodes[id]);
}

void MemoryPool::free_node(int id) {
    if (id >= size) {
        //std::cout << "range out of memory pool size " << id << "," << index << "," << size << std::endl;
        //exit(1);
        FAILED_ASSERT("range out of memory pool size");
    }
    if (nodes[id].id == -1) {
        //std::cout << "node not generated yet" << std::endl;
        //exit(1);
        FAILED_ASSERT("node not generated yet");
    }
    nodes[id].reset();
    index--;
}

void MemoryPool::reset() {
    index = 0;
    label++;
}

MemoryPool::~MemoryPool() {
    if (nodes != nullptr) {
        delete[] nodes;
        nodes = nullptr;
    }
}

#endif
