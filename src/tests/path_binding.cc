#include <iostream>
#include <string>
#include <vector>

#include "query/executor/binding_iter/gql/path_binding.h"

// PathBinding::concat on the graph  n1 -e1-> n2.
//
// A GQL path pattern only produces fragments that start and end with a node, so the GQL tests only
// join two nodes. This tests what they cannot reach: the joins that involve an edge, and the
// empty path.

typedef bool TestFunction();

const ObjectId dir_right(ObjectId::DIRECTION_RIGHT);
const ObjectId dir_left(ObjectId::DIRECTION_LEFT);
const ObjectId dir_undirected(ObjectId::DIRECTION_UNDIRECTED);

// The last digit of an id is the number in the name of the element
const PathElement n1 = PathElement::node(ObjectId(11));
const PathElement n2 = PathElement::node(ObjectId(12));
const PathElement e1 = PathElement::edge(ObjectId(21), dir_right, n1.id, n2.id);

// The variables are printed as the letters a, b, x and y
const VarId a(0);
const VarId b(1);
const VarId x(23);
const VarId y(24);

// The path that has the given elements, each with one variable
PathBinding path(const std::vector<PathElement>& elements, const std::vector<VarId>& vars)
{
    PathBinding res;
    for (size_t i = 0; i < elements.size(); i++) {
        res.push_right(elements[i], vars[i]);
    }
    return res;
}

// The elements of a path and their variables, for example "n1{a} e1{x,y} n2{b}"
std::string to_string(const PathBinding& path)
{
    std::string res;
    for (auto& [element, vars] : path.elements) {
        res += res.empty() ? "" : " ";
        res += (element.is_edge ? "e" : "n") + std::to_string(element.id.id % 10) + "{";
        for (auto& var : vars) {
            res += res.back() == '{' ? "" : ",";
            res += static_cast<char>('a' + var.id);
        }
        res += "}";
    }
    return res;
}

bool differs(const std::string& test, const PathBinding& received, const std::string& expected)
{
    const auto received_str = to_string(received);
    if (received_str != expected) {
        std::cerr << test << ": received " << received_str << ", expected " << expected << "\n";
        return true;
    }
    return false;
}

// (a:n1)-[x:e1]-> · -[y:e1]->(b:n2): e1 is one edge with both variables
bool edge_edge()
{
    auto res = path({ n1, e1 }, { a, x });
    res.concat(path({ e1, n2 }, { y, b }));

    return differs("edge·edge", res, "n1{a} e1{x,y} n2{b}");
}

// (a:n1) · -[x:e1]->(b:n2): the edge starts at the node
bool node_edge()
{
    auto res = path({ n1 }, { a });
    res.concat(path({ e1, n2 }, { x, b }));

    return differs("node·edge", res, "n1{a} e1{x} n2{b}");
}

// The empty path is the identity
bool empty_path()
{
    auto res1 = path({ n1, e1, n2 }, { a, x, b });
    res1.concat(PathBinding());

    PathBinding res2;
    res2.concat(path({ n1, e1, n2 }, { a, x, b }));

    bool error = differs("path·empty", res1, "n1{a} e1{x} n2{b}");
    error |= differs("empty·path", res2, "n1{a} e1{x} n2{b}");
    return error;
}

// Returns true, and says which check it was, if the condition does not hold
bool fails(bool condition, const std::string& check)
{
    if (!condition) {
        std::cerr << "PathElement: " << check << "\n";
    }
    return !condition;
}

// Whether two elements differ and operator< puts them in an order
bool distinct(const PathElement& lhs, const PathElement& rhs)
{
    return lhs != rhs && (lhs < rhs) != (rhs < lhs);
}

// Two elements differ if their ids, their directions or their endpoints differ
bool path_element()
{
    // the two ways through a self-loop only differ in the direction
    const PathElement loop = PathElement::edge(ObjectId(23), dir_right, n1.id, n1.id);
    const PathElement loop_back = PathElement::edge(loop.id, dir_left, n1.id, n1.id);

    // the two ways through an undirected edge only differ in the endpoints
    const PathElement und = PathElement::edge(ObjectId(24), dir_undirected, n1.id, n2.id);
    const PathElement und_back = PathElement::edge(und.id, dir_undirected, n2.id, n1.id);

    bool error = fails(distinct(n1, n2), "two nodes differ");
    error |= fails(distinct(loop, loop_back), "the two ways through a self-loop differ");
    error |= fails(distinct(und, und_back), "the two ways through an undirected edge differ");
    return error;
}

int main()
{
    std::vector<TestFunction*> tests;

    tests.push_back(&edge_edge);
    tests.push_back(&node_edge);
    tests.push_back(&empty_path);
    tests.push_back(&path_element);

    auto error = false;

    for (auto& test_func : tests) {
        if (test_func()) {
            error = true;
        }
    }

    return error;
}
