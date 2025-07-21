#ifdef ENABLE_ROTATE_MODEL

#include <environment/state.hpp>

uint64_t State::Hasher::operator()(const State &n) const {
    uint64_t loc_hash = std::hash<int>()(n.location);
    uint64_t time_hash = std::hash<int>()(n.timestep);
    uint64_t ori_hash = std::hash<int>()(n.orientation);
    return (time_hash ^ (loc_hash << 1) ^ (ori_hash << 2));
}

bool State::operator==(const State &other) const {
    return timestep == other.timestep && location == other.location && orientation == other.orientation;
}

bool State::operator!=(const State &other) const {
    return timestep != other.timestep || location != other.location || orientation != other.orientation;
}

State::State(int location, int timestep, int orientation) : location(location), timestep(timestep), orientation(orientation) {}

#endif