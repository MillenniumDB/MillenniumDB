#pragma once

#include <cassert>
#include <memory>
#include <ostream>
#include <set>
#include <vector>

#include "path_element.h"
#include "query/query_context.h"
#include "query/var_id.h"

// A path with the variables bound to its elements: the result of a path pattern or of a fragment.
//
// It is an annotated path of Arenas, Libkin and Martens, "A Compositional Language for Property
// Graphs" (arXiv 2606.23399): nodes and edges alternate, and a path may start and end with a node
// or with an edge, or be empty. GQL path patterns only match paths that start and end with a node.
class PathBinding {
public:
    // An element of the path and the variables of the pattern that are bound to it
    struct BoundElement {
        PathElement element;
        std::set<VarId> vars;
    };

    std::vector<BoundElement> elements;

    // The element at which the path ends
    const PathElement& end_element() const
    {
        return elements.back().element;
    }

    void push_right(const PathElement& element, VarId var)
    {
        // nodes and edges alternate, and an edge starts and ends at the nodes next to it
        assert(elements.empty() || elements.back().element.is_edge != element.is_edge);
        assert(elements.empty() || elements.back().element.end_node() == element.start_node());
        elements.push_back({ element, { var } });
    }

    // Concatenates other to the right of this path. The last element of this path and the first
    // element of other are joined as annotated elements:
    //   - node·node and edge·edge: both are the same element, which gets the variables of both
    //       this   : (a:n1)-[x:e1]->(b:n2)
    //       other  : (c:n2)-[y:e2]->(d:n3)
    //       result : (a:n1)-[x:e1]->([b,c]:n2)-[y:e2]->(d:n3)
    //   - node·edge and edge·node: the edge starts, or ends, at the node
    // The empty path is the identity. The iterators only return paths that can be joined, as each
    // fragment starts where the fragment on its left ends (see PathBindingIter::set_left_boundary).
    void concat(const PathBinding& other)
    {
        if (other.elements.empty()) {
            return;
        }

        auto first = other.elements.begin();
        if (!elements.empty()) {
            BoundElement& last = elements.back();
            if (last.element.is_edge == first->element.is_edge) {
                assert(last.element == first->element);
                last.vars.insert(first->vars.begin(), first->vars.end());
                ++first;
            } else {
                assert(last.element.end_node() == first->element.start_node());
            }
        }
        elements.insert(elements.end(), first, other.elements.end());
    }

    std::unique_ptr<PathBinding> clone() const
    {
        return std::make_unique<PathBinding>(*this);
    }

    void debug_print(std::ostream& os) const
    {
        if (elements.empty()) {
            os << "EMPTY_PATH_BINDING";
        }

        for (auto& [element, vars] : elements) {
            const char* open = "(";
            const char* close = ")";
            if (element.is_edge) {
                switch (element.direction.id) {
                case ObjectId::DIRECTION_LEFT:
                    open = "<-[";
                    close = "]-";
                    break;
                case ObjectId::DIRECTION_RIGHT:
                    open = "-[";
                    close = "]->";
                    break;
                default:
                    open = "~[";
                    close = "]~";
                    break;
                }
            }

            os << open;
            bool first = true;
            for (auto& var : vars) {
                os << (first ? "" : ",") << var;
                first = false;
            }
            os << ":" << element.id << close;
        }
    }
};
