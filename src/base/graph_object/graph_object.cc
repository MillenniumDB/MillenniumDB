#include "graph_object.h"

int (*GraphObject::graph_object_cmp)(const GraphObject&, const GraphObject&);
bool (*GraphObject::graph_object_eq)(const GraphObject&, const GraphObject&);

GraphObject (*GraphObject::graph_object_sum)(const GraphObject&, const GraphObject&);
GraphObject (*GraphObject::graph_object_minus)(const GraphObject&, const GraphObject&);
GraphObject (*GraphObject::graph_object_multiply)(const GraphObject&, const GraphObject&);
GraphObject (*GraphObject::graph_object_divide)(const GraphObject&, const GraphObject&);
GraphObject (*GraphObject::graph_object_modulo)(const GraphObject&, const GraphObject&);

std::ostream& (*GraphObject::graph_object_print)(std::ostream&, const GraphObject&);
