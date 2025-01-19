#pragma once

#include <cstdint>
#include <map>
#include <ostream>

namespace GQL {
class PathMode {
public:
    enum Prefix {
        ALL,
        ANY,
        ALL_SHORTEST,
        ANY_SHORTEST,
        SHORTEST_PATH_COUNT,
        SHORTEST_GROUP_COUNT,
    };

    enum Mode {
        WALK,
        TRAIL,
        SIMPLE,
        ACYCLIC,
        DEFAULT,
    };

    Prefix prefix = ANY;
    Mode mode = DEFAULT;
    uint64_t path_count = 1;

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const
    {
        std::map<Prefix, std::string> prefix_to_str = {
            {                  ALL,                  "ALL" },
            {                  ANY,                  "ANY" },
            {         ALL_SHORTEST,         "ALL_SHORTEST" },
            {         ANY_SHORTEST,         "ANY_SHORTEST" },
            {  SHORTEST_PATH_COUNT,  "SHORTEST_PATH_COUNT" },
            { SHORTEST_GROUP_COUNT, "SHORTEST_GROUP_COUNT" },
        };

        std::map<Mode, std::string> mode_to_str = {
            {    WALK,    "WALK" },
            {   TRAIL,   "TRAIL" },
            {  SIMPLE,  "SIMPLE" },
            { ACYCLIC, "ACYCLIC" },
            { DEFAULT, "DEFAULT" },
        };

        os << std::string(indent, ' ');
        os << "Mode(" << prefix_to_str[prefix] << ", ";
        os << path_count << ", ";
        os << mode_to_str[mode] << ")";
        return os;
    }
};
} // namespace GQL
