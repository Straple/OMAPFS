#include <planner/wppl/LNS/Parallel/DataStructure.h>
#ifdef ENABLE_ROTATE_MODEL

namespace LNS {

    namespace Parallel {

        std::ostream &operator<<(std::ostream &out, const PathEntry &pe) {
            out << pe.location << "," << pe.orientation;
            return out;
        }

        std::ostream &operator<<(std::ostream &out, const Path &path) {
            for (auto &pe: path.nodes) {
                out << "(" << pe.location << "," << pe.orientation << ")->";
            }
            out << std::endl;
            return out;
        }

    }// namespace Parallel

}// namespace LNS
#endif
