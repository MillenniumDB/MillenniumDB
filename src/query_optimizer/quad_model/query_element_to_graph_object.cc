#include "query_element_to_graph_object.h"

#include "base/exceptions.h"
#include "execution/graph_object/graph_object_factory.h"


GraphObject QueryElementToGraphObject::operator()(const QueryElement& obj) {
    return std::visit(*this, obj.value);
}


GraphObject QueryElementToGraphObject::operator()(const Var&) {
    throw LogicException("Var cannot be converted into GraphObject");
}


GraphObject QueryElementToGraphObject::operator()(const NamedNode& named_node) {
    if (named_node.name.size() < 8) {
        char c[8];
        std::size_t i = 0;
        for (; i < named_node.name.size(); ++i) {
            c[i] = named_node.name[i];
        }
        for (; i < 8; ++i) {
            c[i] = '\0';
        }
        return GraphObjectFactory::make_named_node_inlined(c);
    } else {
        // TODO: check if it exists?
        return GraphObjectFactory::make_named_node_tmp(named_node.name);
    }
}


GraphObject QueryElementToGraphObject::operator()(const std::string& str) {
    if (str.size() < 8) {
        char c[8];
        std::size_t i = 0;
        for (; i < str.size(); ++i) {
            c[i] = str[i];
        }
        for (; i < 8; ++i) {
            c[i] = '\0';
        }
        return GraphObjectFactory::make_string_inlined(c);
    } else {
        // TODO: check if it exists?
        return GraphObjectFactory::make_string_tmp(str);
    }
}


GraphObject QueryElementToGraphObject::operator()(const AnonymousNode& anon) {
    return GraphObjectFactory::make_anonymous(anon.id);
}


GraphObject QueryElementToGraphObject::operator()(const Edge& edge) {
    return GraphObjectFactory::make_edge(edge.id);
}


GraphObject QueryElementToGraphObject::operator()(const bool b) {
    return GraphObjectFactory::make_bool(b);
}


GraphObject QueryElementToGraphObject::operator()(int64_t i) {
    return GraphObjectFactory::make_int(i);
}


GraphObject QueryElementToGraphObject::operator()(const float f) {
    return GraphObjectFactory::make_float(f);
}
