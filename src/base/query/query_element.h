#pragma once

#include <cassert>
#include <ostream>
#include <string>
#include <variant>

#include "base/exceptions.h"
#include "base/graph_object/graph_object.h"
#include "base/query/named_node.h"
#include "base/query/query_element_to_string.h"
#include "base/query/var.h"

// TODO: move?
class QueryElement {
public:
    std::variant<Var, NamedNode, AnonymousNode, Edge, std::string, bool, int64_t, float> value;

    QueryElement(Var&& var) : value(std::move(var)) { }

    QueryElement(const Var& var) : value(var) { }

    QueryElement(NamedNode&& node_name) : value(std::move(node_name)) { }

    QueryElement(const NamedNode& node_name) : value(node_name) { }

    QueryElement(std::string&& value) : value(std::move(value)) { }

    QueryElement(const std::string& value) : value(std::move(value)) { }

    QueryElement(AnonymousNode anon) : value(anon) { }

    QueryElement(Edge edge) : value(edge) { }

    explicit QueryElement(bool value) : value(value) { }

    explicit QueryElement(int64_t value) : value(value) { }

    explicit QueryElement(float value) : value(value) { }

    static QueryElement deduce(const std::string& str) {
        assert(!str.empty());
        switch (str[0]) {
        case '?': {
            return QueryElement(Var(move(str)));
        }
        case '"': {
            assert(str.size() >= 2);
            std::string tmp = str.substr(1, str.size() - 2); // delete first and last characters: ("")
            return QueryElement(std::move(tmp));
        }
        case '_': {
            assert(str.size() >= 3);
            std::string tmp = str.substr(2, str.size() - 2); // delete first 2 characters "_a" or "_e"
            if (str[1] == 'a') {
                return QueryElement(AnonymousNode(std::stoi(tmp)));
            } else {
                // (str[1] == 'e')
                return QueryElement(Edge(std::stoi(tmp)));
            }
        }
        case 't': {
            if (str == "true") {
                return QueryElement(true);
            } else {
                return QueryElement(NamedNode(str));
            }
        }
        case 'f': {
            if (str == "false") {
                return QueryElement(false);
            } else {
                return QueryElement(NamedNode(str));
            }
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '+':
        case '-':
            // TODO: support other forms of float (".01") ?
            if (str.find_first_not_of("0123456789+-") == std::string::npos) {
                // is int
                return QueryElement(std::stol(str));
            } else {
                // is float
                return QueryElement(std::stof(str));
            }

        default:
            return QueryElement(NamedNode(str));
        }
    }

    inline bool operator==(const QueryElement& rhs) const noexcept {
        return value == rhs.value;
    }

    inline bool operator!=(const QueryElement& rhs) const noexcept {
        return !(*this == rhs);
    }

    inline bool operator<(const QueryElement& rhs) const noexcept {
        return value < rhs.value;
    }

    inline bool is_var() const {
        return std::holds_alternative<Var>(value);
    }

    inline std::string to_string() const {
        return std::visit(QueryElementToString(), value);
    }

    inline Var to_var() const {
        assert(std::holds_alternative<Var>(value));
        return std::get<Var>(value);
    }

    friend std::ostream& operator<<(std::ostream& os, const QueryElement& node_id) {
        return os << node_id.to_string();
    }
};
