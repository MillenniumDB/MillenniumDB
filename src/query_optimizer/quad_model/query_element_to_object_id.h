#pragma once

#include "base/ids/object_id.h"
#include "base/query/query_element.h"

struct QueryElementToObjectId {
    QueryElementToObjectId(bool create_if_not_exists = false) : create_if_not_exists(create_if_not_exists) { }

    ObjectId operator()(const QueryElement&);

    ObjectId operator()(const Var&);
    ObjectId operator()(const NamedNode&);
    ObjectId operator()(const AnonymousNode&);
    ObjectId operator()(const Edge&);
    ObjectId operator()(const std::string&);
    ObjectId operator()(bool);
    ObjectId operator()(int64_t);
    ObjectId operator()(float);

private:
    bool create_if_not_exists;

    ObjectId get_string_id(const std::string&, uint64_t mask_inlined, uint64_t mask_external);
};
