#pragma once

#include <list>
#include <memory>
#include <ostream>

#include "path_step.h"
#include "query/query_context.h"

// Represents a path which is the result of a GQL query. Each element in the list is a step, meaning that it
// contains a node and an edge. The first step of the list does not contain an edge, so that a path can be
// constructed by alternating nodes and edges.
class PathBinding {
public:
    // Each element contains a set of nodes and an edge, except the first one which contains only the nodes.
    std::list<PathStep> steps;

    void push_left(PathStep element)
    {
        steps.push_front(element);
    }

    void push_right(PathStep element)
    {
        steps.push_back(element);
    }

    PathStep& front()
    {
        return steps.front();
    }

    PathStep& back()
    {
        return steps.back();
    }

    // adds the first vars of other to the last step in this path binding.
    // concatenates starting from the second step of the other path binding.
    // this before  : (a:n1)-[x:e1]->(b:n2)
    // this after   : (a:n1)-[x:e1]->([b,c]:n2)-...
    // c is the first var of the binding list.
    void merge_right(const std::unique_ptr<PathBinding>& other)
    {
        if (other->steps.empty() || steps.empty()) {
            return;
        }

        // copy the node vars
        std::set<VarId>& this_vars = steps.back().node_vars;
        std::set<VarId>& next_vars = other->front().node_vars;
        this_vars.merge(next_vars);

        // extend the list
        if (other->size() > 1) {
            auto pos = other->steps.begin();
            pos++;
            steps.insert(steps.end(), pos, other->steps.end());
        }
    }

    // adds the last vars of other to the first step in this path binding.
    // concatenates starting from the second to last step of the other path binding.
    // also adds the last edge var and edge values of other to the first step of this list.
    void merge_left(const std::unique_ptr<PathBinding>& other)
    {
        if (other->steps.empty() || steps.empty()) {
            return;
        }

        // copy the node vars
        std::set<VarId>& this_vars = steps.front().node_vars;
        std::set<VarId>& prev_vars = other->back().node_vars;
        this_vars.merge(prev_vars);

        if (other->size() > 1) {
            // copy the edge
            steps.front().edge_var = other->back().edge_var;
            steps.front().edge_value = other->back().edge_value;

            // extend the list
            auto pos = other->steps.end();
            pos--;
            steps.insert(steps.begin(), other->steps.begin(), pos);
        }
    }

    uint64_t size()
    {
        return steps.size();
    }

    std::unique_ptr<PathBinding> clone()
    {
        return std::make_unique<PathBinding>(*this);
    }

    void clear()
    {
        steps.clear();
    }

    void debug_print(std::ostream& os)
    {
        if (steps.empty()) {
            os << "EMPTY_PATH_BINDING";
        }

        for (auto it = steps.begin(); it != steps.end(); ++it) {
            switch (it->edge_direction.id) {
            case ObjectId::DIRECTION_LEFT:
                os << "<-[" << it->edge_var << ":" << it->edge_value << "]-";
                break;
            case ObjectId::DIRECTION_RIGHT:
                os << "-[" << it->edge_var << ":" << it->edge_value << "]->";
                break;
            case ObjectId::DIRECTION_UNDIRECTED:
                os << "~[" << it->edge_var << ":" << it->edge_value << "]~";
                break;
            default:
                break;
            }

            os << "(";
            bool first = true;
            for (auto& var : it->node_vars) {
                if (first) {
                    first = false;
                    os << var;
                } else {
                    os << "," << var;
                }
            }
            os << ":" << it->node_value << ")";
        }
    }
};
