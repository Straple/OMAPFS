#include <planner/wppl/RHCR/main/ECBSNode.h>
#ifdef ENABLE_ROTATE_MODEL

namespace RHCR {

    ECBSNode::ECBSNode(ECBSNode *parent) : parent(parent) {
        g_val = parent->g_val;
        h_val = 0;
        min_f_val = parent->min_f_val;
        depth = parent->depth + 1;
    }

    void ECBSNode::clear() {
        conflicts.clear();
    }
}// namespace RHCR
#endif
