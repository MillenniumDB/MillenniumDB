#pragma once

#include <ostream>
#include <type_traits>
#include <iostream>

enum class GraphObjectType;

class GraphObject {
friend class GraphObjectFactory;
public:
    uint64_t encoded_value;

    GraphObjectType type;

    static int (*graph_object_cmp)(const GraphObject&, const GraphObject&);
    static bool (*graph_object_eq)(const GraphObject&, const GraphObject&);
    static std::ostream& (*graph_object_print)(std::ostream&, const GraphObject&);
    static GraphObject (*graph_object_sum)     (const GraphObject&, const GraphObject&);
    static GraphObject (*graph_object_minus)   (const GraphObject&, const GraphObject&);
    static GraphObject (*graph_object_multiply)(const GraphObject&, const GraphObject&);
    static GraphObject (*graph_object_divide)  (const GraphObject&, const GraphObject&);
    static GraphObject (*graph_object_modulo)  (const GraphObject&, const GraphObject&);

    GraphObject() : encoded_value(0), type(GraphObjectType(0)) { }

    friend std::ostream& operator<<(std::ostream& os, const GraphObject& graph_obj) {
        return graph_object_print(os, graph_obj);
    }

    inline bool operator==(const GraphObject& rhs) const noexcept {
        return graph_object_eq(*this, rhs);
    }

    inline bool operator!=(const GraphObject& rhs) const noexcept {
        return !graph_object_eq(*this, rhs);
    }

    inline bool operator<=(const GraphObject& rhs) const noexcept {
        return graph_object_cmp(*this, rhs) <= 0;
    }

    inline bool operator<(const GraphObject& rhs) const noexcept {
        return graph_object_cmp(*this, rhs) < 0;
    }

    inline bool operator>=(const GraphObject& rhs) const noexcept {
        return graph_object_cmp(*this, rhs) >= 0;
    }

    inline bool operator>(const GraphObject& rhs) const noexcept {
        return graph_object_cmp(*this, rhs) > 0;
    }

    inline GraphObject operator+(const GraphObject& rhs) const noexcept {
        return graph_object_sum(*this, rhs);
    }

    inline GraphObject operator-(const GraphObject& rhs) const noexcept {
        return graph_object_minus(*this, rhs);
    }

    inline GraphObject operator*(const GraphObject& rhs) const noexcept {
        return graph_object_multiply(*this, rhs);
    }

    inline GraphObject operator/(const GraphObject& rhs) const noexcept {
        return graph_object_divide(*this, rhs);
    }

    inline GraphObject operator%(const GraphObject& rhs) const noexcept {
        return graph_object_modulo(*this, rhs);
    }

private:
    GraphObject(int64_t encoded_value, GraphObjectType type) : encoded_value(encoded_value), type(type) { }
};

class GraphObjectInterpreter {
public:
    template<typename T>
    static T get(const GraphObject graph_object);
};

static_assert(std::is_trivially_copyable<GraphObject>::value);

static_assert(std::is_trivially_assignable<GraphObject, GraphObject>::value);

static_assert(std::is_trivially_move_constructible<GraphObject>::value);

static_assert(sizeof(GraphObject) <= 16,
              "GraphObject size should be small, if it needs to increase the size avoid copies");
