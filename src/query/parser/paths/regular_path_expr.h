#pragma once

#include <memory>
#include <ostream>
#include <string>

#include "query/parser/paths/automaton/rpq_automaton.h"
#include "query/parser/paths/automaton/rdpq_automaton.h"

enum class PathType {
    PATH_ALTERNATIVES,
    PATH_SEQUENCE,
    PATH_KLEENE_STAR,
    PATH_KLEENE_PLUS,
    PATH_ATOM,
    PATH_OPTIONAL,
    PATH_CHECK,
    PATH_NEGATED_SET,
};

enum class PathSemantic {
    ALL_ACYCLIC,
    ALL_SIMPLE,
    ALL_TRAILS,
    // ALL_WALKS not supported
    ALL_SHORTEST_ACYCLIC,
    ALL_SHORTEST_SIMPLE,
    ALL_SHORTEST_TRAILS,
    ALL_SHORTEST_WALKS,
    ALL_SHORTEST_WALKS_COUNT,
    ANY_ACYCLIC,
    ANY_SIMPLE,
    ANY_TRAILS,
    ANY_WALKS,
    ANY_SHORTEST_ACYCLIC,
    ANY_SHORTEST_SIMPLE,
    ANY_SHORTEST_TRAILS,
    ANY_SHORTEST_WALKS,
    SHORTEST_K_ACYCLIC,
    SHORTEST_K_SIMPLE,
    SHORTEST_K_TRAILS,
    SHORTEST_K_WALKS,
    SHORTEST_K_GROUPS_ACYCLIC,
    SHORTEST_K_GROUPS_SIMPLE,
    SHORTEST_K_GROUPS_TRAILS,
    SHORTEST_K_GROUPS_WALKS,
    DEFAULT,
};

namespace Paths {
    inline const char* get_semantic_str(PathSemantic s) {
        switch (s) {
            case PathSemantic::ALL_ACYCLIC: return "ALL_ACYCLIC";
            case PathSemantic::ALL_SIMPLE: return "ALL_SIMPLE";
            case PathSemantic::ALL_TRAILS: return "ALL_TRAILS";
            case PathSemantic::ALL_SHORTEST_ACYCLIC: return "ALL_SHORTEST_ACYCLIC";
            case PathSemantic::ALL_SHORTEST_SIMPLE: return "ALL_SHORTEST_SIMPLE";
            case PathSemantic::ALL_SHORTEST_TRAILS: return "ALL_SHORTEST_TRAILS";
            case PathSemantic::ALL_SHORTEST_WALKS: return "ALL_SHORTEST_WALKS";
            case PathSemantic::ALL_SHORTEST_WALKS_COUNT: return "ALL_SHORTEST_WALKS_COUNT";
            case PathSemantic::ANY_ACYCLIC: return "ANY_ACYCLIC";
            case PathSemantic::ANY_SIMPLE: return "ANY_SIMPLE";
            case PathSemantic::ANY_TRAILS: return "ANY_TRAILS";
            case PathSemantic::ANY_WALKS: return "ANY_WALKS";
            case PathSemantic::ANY_SHORTEST_ACYCLIC: return "ANY_SHORTEST_ACYCLIC";
            case PathSemantic::ANY_SHORTEST_SIMPLE: return "ANY_SHORTEST_SIMPLE";
            case PathSemantic::ANY_SHORTEST_TRAILS: return "ANY_SHORTEST_TRAILS";
            case PathSemantic::ANY_SHORTEST_WALKS: return "ANY_SHORTEST_WALKS";
            case PathSemantic::SHORTEST_K_ACYCLIC: return "SHORTEST_K_ACYCLIC";
            case PathSemantic::SHORTEST_K_SIMPLE: return "SHORTEST_K_SIMPLE";
            case PathSemantic::SHORTEST_K_TRAILS: return "SHORTEST_K_TRAILS";
            case PathSemantic::SHORTEST_K_WALKS: return "SHORTEST_K_WALKS";
            case PathSemantic::SHORTEST_K_GROUPS_ACYCLIC: return "SHORTEST_K_GROUPS_ACYCLIC";
            case PathSemantic::SHORTEST_K_GROUPS_SIMPLE: return "SHORTEST_K_GROUPS_SIMPLE";
            case PathSemantic::SHORTEST_K_GROUPS_TRAILS: return "SHORTEST_K_GROUPS_TRAILS";
            case PathSemantic::SHORTEST_K_GROUPS_WALKS: return "SHORTEST_K_GROUPS_WALKS";
            case PathSemantic::DEFAULT: return "DEFAULT";
            default: return "UNDECLARED PATH SEMANTIC";
        }
    }
}

// Path-finding modes for operators
enum class PathSearchMode {
    BFS,  // Breadth-first search
    DFS   // Depth-first search
};

class RegularPathExpr {
public:
    virtual ~RegularPathExpr() = default;

    virtual std::unique_ptr<RegularPathExpr> clone() const = 0;

    virtual PathType type() const = 0;

    virtual std::string to_string() const = 0;

    virtual std::unique_ptr<RegularPathExpr> invert() const = 0;

    RPQ_DFA get_rpq_automaton(ObjectId(*str_to_oid)(const std::string&)) const {
        auto automaton = get_rpq_base_automaton();
        return automaton.transform_automaton(str_to_oid);
    }

    RDPQAutomaton get_rdpq_automaton(ObjectId(*str_to_oid)(const std::string&)) const {
        auto automaton = get_rdpq_base_automaton();
        automaton.transform_automaton(str_to_oid);
        return automaton;
    }

    friend std::ostream& operator<<(std::ostream& os, const RegularPathExpr& b) {
        return b.print_to_ostream(os);
    }

    bool operator<(const RegularPathExpr& other) const {
        return to_string() < other.to_string();
    }

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    // true if the path can be empty
    virtual bool nullable() const = 0;

    virtual RPQ_NFA get_rpq_base_automaton() const = 0;

    virtual RDPQAutomaton get_rdpq_base_automaton() const = 0;
};