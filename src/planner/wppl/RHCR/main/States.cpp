#include <planner/wppl/RHCR/main/States.h>
#ifdef ENABLE_ROTATE_MODEL

namespace RHCR {

    std::ostream &operator<<(std::ostream &out, const State &s) {
        out << s.location << "," << s.orientation << "," << s.timestep;
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const Path &path) {
        for (auto state: path) {
            if (state.location < 0)
                continue;
            out << "(" << state.location << "," << state.orientation << "," << state.timestep << ")->";
        }
        out << std::endl;
        return out;
    }
}// namespace RHCR
#endif
