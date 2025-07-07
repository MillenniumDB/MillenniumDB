#pragma once

#include <cstdint>
#include <map>
#include <ostream>

namespace GQL {
class PathMode {
public:
    enum Selector {
        ALL,
        ANY,
        ALL_SHORTEST,
        ANY_SHORTEST,
        SHORTEST_PATH_COUNT,
        SHORTEST_GROUP_COUNT,
    };

    enum Restrictor {
        WALK,
        TRAIL,
        SIMPLE,
        ACYCLIC,
        DEFAULT,
    };

    Selector selector = ALL;
    Restrictor restrictor = DEFAULT;
    uint64_t path_count = 1;

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const
    {
        std::map<Selector, std::string> selector_to_str = {
            {                  ALL,                  "ALL" },
            {                  ANY,                  "ANY" },
            {         ALL_SHORTEST,         "ALL_SHORTEST" },
            {         ANY_SHORTEST,         "ANY_SHORTEST" },
            {  SHORTEST_PATH_COUNT,  "SHORTEST_PATH_COUNT" },
            { SHORTEST_GROUP_COUNT, "SHORTEST_GROUP_COUNT" },
        };

        std::map<Restrictor, std::string> prefix_to_str = {
            {    WALK,    "WALK" },
            {   TRAIL,   "TRAIL" },
            {  SIMPLE,  "SIMPLE" },
            { ACYCLIC, "ACYCLIC" },
            { DEFAULT, "DEFAULT" },
        };

        os << std::string(indent, ' ');
        os << "Mode(" << selector_to_str[selector] << ", ";
        os << path_count << ", ";
        os << prefix_to_str[restrictor] << ")";
        return os;
    }
};
} // namespace GQL
