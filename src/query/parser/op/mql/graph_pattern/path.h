#pragma once

#include <memory>
#include <string>

#include "query/id.h"
#include "query/parser/paths/regular_path_expr.h"

namespace MQL {

class Path {
public:
    enum class Direction {
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT
    };

    const VarId var;
    const Id from;
    const Id to;
    const PathSemantic semantic;
    const Direction direction;

    // used only in SHORTEST K semantics, ignored in others
    const uint64_t K;

    std::unique_ptr<RegularPathExpr> path;

    Path(
        VarId var,
        Id from,
        Id to,
        PathSemantic semantic,
        Direction direction,
        uint64_t K,
        std::unique_ptr<RegularPathExpr> path
    ) :
        var(var),
        from(from),
        to(to),
        semantic(semantic),
        direction(direction),
        K(K),
        path(std::move(path))
    { }

    Path(const Path& other) :
        var(other.var),
        from(other.from),
        to(other.to),
        semantic(other.semantic),
        direction(other.direction),
        K(other.K),
        path(other.path->clone())
    { }

    bool operator<(const Path& other) const
    {
        if (var < other.var) {
            return true;
        } else if (other.var < var) {
            return false;
        } else if (from.get_OID() < other.from.get_OID()) {
            return true;
        } else if (other.from.get_OID() < from.get_OID()) {
            return false;
        } else if (to.get_OID() < other.to.get_OID()) {
            return true;
        } else if (other.to.get_OID() < to.get_OID()) {
            return false;
        } else if (semantic < other.semantic) {
            return true;
        } else if (other.semantic < semantic) {
            return false;
        } else if (direction < other.direction) {
            return true;
        } else if (other.direction < direction) {
            return false;
        } else if (*path < *other.path) {
            return true;
        } else if (*other.path < *path) {
            return false;
        }

        return false;
    }

    std::ostream& print(std::ostream& os, int indent = 0) const
    {
        return os << std::string(indent, ' ') << "Path(" << Paths::get_semantic_str(semantic) << ", "
                  << var << ", " << from << ", " << to << ", "
                  << path->to_string() << ")\n";
    }
};
} // namespace MQL
