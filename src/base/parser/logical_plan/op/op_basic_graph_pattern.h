#ifndef BASE__OP_BASIC_GRAPH_PATTERN_H_
#define BASE__OP_BASIC_GRAPH_PATTERN_H_

#include <set>
#include <vector>

#include "base/exceptions.h"
#include "base/ids/node_id.h"
#include "base/parser/logical_plan/op/op_connection.h"
#include "base/parser/logical_plan/op/op_label.h"
#include "base/parser/logical_plan/op/op_property.h"
#include "base/parser/logical_plan/op/op_property_path.h"

#include "base/parser/logical_plan/op/op_isolated_term.h"
#include "base/parser/logical_plan/op/op_isolated_var.h"
#include "base/parser/logical_plan/op/property_paths/path_constructor.h"

class OpBasicGraphPattern : public Op {
public:
    std::set<OpLabel>        labels;
    std::set<OpProperty>     properties;
    std::set<OpConnection>   connections;
    std::set<OpPropertyPath> property_paths;
    std::set<OpIsolatedVar>  isolated_vars;
    std::set<OpIsolatedTerm> isolated_terms;

    std::set<Var> vars; // contains declared variables and anonymous (auto-generated in the constructor)

    uint_fast32_t* anon_count; // pointer to a global count of anonymous variables

    OpBasicGraphPattern(const std::vector<query::ast::LinearPattern>& graph_pattern, uint_fast32_t* _anon_count) :
        anon_count(_anon_count)
    {
        std::vector<Var> pending_unjoint_vars;
        for (auto& linear_pattern : graph_pattern) {
            // UnjointObjects will be processed at the end
            if (   linear_pattern.path.empty()
                && linear_pattern.root.labels.empty()
                && linear_pattern.root.properties.empty())
            {
                if (linear_pattern.root.id.type() == typeid(std::string)) {
                    std::string str = boost::get<std::string>(linear_pattern.root.id);
                    if (str.empty()) { // anonymous variable
                        const std::string s = "?_" + std::to_string((*anon_count)++);
                        vars.emplace(s);
                        isolated_vars.insert(OpIsolatedVar(Var(s)));
                    } else if (str[0] == '?') { // explicit variable
                        pending_unjoint_vars.emplace_back(str);
                    } else {
                        isolated_terms.insert(get_node_id(linear_pattern.root.id));
                    }
                } else {
                    isolated_terms.insert(get_node_id(linear_pattern.root.id));
                }
            }
            else {
                auto last_node_id = process_node(linear_pattern.root);
                for (auto& linear_pattern_step : linear_pattern.path) {
                    auto current_node_id = process_node(linear_pattern_step.node);

                    if (linear_pattern_step.path.type() == typeid(query::ast::Edge)) {
                        // EDGE
                        auto edge = boost::get<query::ast::Edge>(linear_pattern_step.path);
                        std::vector<NodeId> types;
                        auto edge_id = process_edge(edge, types);

                        connections.insert(
                            edge.direction == query::ast::EdgeDirection::right
                                ? OpConnection(last_node_id, current_node_id, edge_id, std::move(types))
                                : OpConnection(current_node_id, last_node_id, edge_id, std::move(types)));
                    } else {
                        auto property_path = boost::get<query::ast::PropertyPath>(linear_pattern_step.path);

                        Var path_var = property_path.var.empty()
                                ? Var("?_p" + std::to_string((*anon_count)++))
                                : Var(property_path.var);
                        vars.insert(path_var);

                        PathConstructor path_constructor;
                        if (property_path.direction == query::ast::EdgeDirection::right) {
                            property_paths.emplace(path_var,
                                                   last_node_id,
                                                   current_node_id,
                                                   path_constructor(property_path.path_alternatives));
                        } else { // property_path.direction == query::ast::EdgeDirection::left
                            property_paths.emplace(path_var,
                                                   current_node_id,
                                                   last_node_id,
                                                   path_constructor(property_path.path_alternatives));
                        }
                    }
                    last_node_id = current_node_id;
                }
            }
        }
        for (auto& pending_unjoint_var : pending_unjoint_vars) {
            if (vars.insert(pending_unjoint_var).second) {
                isolated_vars.insert(OpIsolatedVar(pending_unjoint_var));
            }
        }
    }


    NodeId get_node_id(const boost::variant<std::string, bool, int64_t, float>& id) {
        if (id.type() == typeid(std::string)) {
            auto str = boost::get<std::string>(id);
            if (str.empty()) { // anonymous variable
                const std::string s = "?_" + std::to_string((*anon_count)++);
                Var v(s);
                vars.insert(v);
                return NodeId(v);
            } else if (str[0] == '?') { // explicit variable
                const Var v(str);
                vars.insert(v);
                return NodeId(v);
            } else if (str[0] == '"') { // string
                std::string tmp = str.substr(1, str.size() - 2); // delete first and last characters: ("")
                return NodeId(std::move(tmp));
            } else if (str[0] == '_') { // anonymous node or connection
                std::string tmp = str.substr(2, str.size() - 2); // delete first 2 characters (_a1234)
                if (str[1] == 'a') {
                    return NodeId(AnonymousNode( std::stoi(tmp) ));
                } else if (str[1] == 'c') {
                    return NodeId(Edge( std::stoi(tmp) ));
                } else {
                    throw QuerySemanticException("Invalid Id");
                }
            } else { // identifier
                return NodeId(NodeName(str));
            }
        } else if (id.type() == typeid(bool)) {
            auto b = boost::get<bool>(id);
            return NodeId(b);
        } else if (id.type() == typeid(int64_t)) {
            auto i = boost::get<int64_t>(id);
            return NodeId(i);
        } else { // if (id.type() == typeid(float)) {
            auto f = boost::get<float>(id);
            return NodeId(f);
        }
    }


    NodeId process_node(const query::ast::Node& node) {
        NodeId node_id = get_node_id(node.id);

        for (auto& label : node.labels) {
            labels.insert(OpLabel(node_id, label));
        }

        for (auto& property : node.properties) {
            auto new_property = OpProperty(node_id, property.key, property.value);
            auto property_search = properties.find(new_property);

            if (property_search != properties.end()) {
                auto old_property = *property_search;
                if (old_property.value != property.value) {
                    throw QuerySemanticException(node_id.to_string() + "." + property.key
                                                 + " its declared with different values");
                }
            }
            else {
                properties.insert(new_property);
            }
        }
        return node_id;
    }


    NodeId process_edge(const query::ast::Edge& edge, std::vector<NodeId>& types) {
        NodeId edge_id = NodeId(Edge(1)); // This value doesn't matter, it will be setted after

        if (edge.id.empty()) {
            edge_id = NodeId(Var("?_c" + std::to_string((*anon_count)++)));
            vars.insert(edge_id.to_var());
        } else if (edge.id[0] == '?') {
            edge_id = NodeId(Var(edge.id));
            vars.insert(edge_id.to_var());
        } else {
            auto tmp = edge.id.substr(2, edge.id.size() - 2);
            edge_id = NodeId(Edge(std::stoi(tmp)));
        }

        for (const auto &type : edge.types) {
            if (type[0] == '?') {
                vars.insert(Var(type));
            }
            types.push_back(NodeId(NodeName(type)));
        }

        if (edge.types.size() == 0) {
            auto tmp = edge_id.to_string();
            if (tmp[0] == '?') {
                tmp.erase(0, 1);
            }
            vars.insert(Var("?_typeof_" + tmp));
        }

        for (auto& property : edge.properties) {
            auto new_property = OpProperty(edge_id, property.key, property.value);
            auto property_search = properties.find(new_property);

            if (property_search != properties.end()) {
                auto old_property = *property_search;
                if (old_property.value != property.value) {
                    throw QuerySemanticException(edge_id.to_string() + "." + property.key
                                                 + " its declared with different values in MATCH");
                }
            } else {
                properties.insert(new_property);
            }
        }

        return edge_id;
    }


    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override{
        os << std::string(indent, ' ');
        os << "OpBasicGraphPattern()\n";

        for (auto& label : labels) {
            label.print_to_ostream(os, indent + 2);
        }
        for (auto& property : properties) {
            property.print_to_ostream(os, indent + 2);
        }
        for (auto& connection : connections) {
            connection.print_to_ostream(os, indent + 2);
        }
        for (auto& property_path : property_paths) {
            property_path.print_to_ostream(os, indent + 2);
        }
        for (auto& isolated_var : isolated_vars) {
            isolated_var.print_to_ostream(os, indent + 2);
        }
        for (auto& isolated_term : isolated_terms) {
            isolated_term.print_to_ostream(os, indent + 2);
        }
        return os;
    }


    void accept_visitor(OpVisitor &visitor) override {
        visitor.visit(*this);
    }


    std::set<Var> get_vars() const override {
        return vars;
    }
};

#endif // BASE__OP_BASIC_GRAPH_PATTERN_H_
