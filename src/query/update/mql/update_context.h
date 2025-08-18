#pragma once

#include <set>

#include "graph_models/object_id.h"
#include "graph_models/quad_model/quad_model.h"

namespace MQL {
struct DeleteObjectInfo {
    ObjectId obj;
    bool detach;

    DeleteObjectInfo(ObjectId obj, bool detach) :
        obj(obj),
        detach(detach)
    { }

    bool operator<(const DeleteObjectInfo& other) const
    {
        if (obj != other.obj) {
            return obj < other.obj;
        }
        return detach < other.detach;
    }
};

struct LabelInfo {
    ObjectId node;
    ObjectId label;

    LabelInfo(ObjectId node, ObjectId label) :
        node(node),
        label(label)
    { }

    bool operator<(const LabelInfo& other) const
    {
        if (node != other.node) {
            return node < other.node;
        }
        return label < other.label;
    }
};

struct NewPropertyInfo {
    ObjectId obj;
    ObjectId key;
    ObjectId val;

    NewPropertyInfo(ObjectId obj, ObjectId key, ObjectId val) :
        obj(obj),
        key(key),
        val(val)
    { }

    bool operator<(const NewPropertyInfo& other) const
    {
        if (obj != other.obj) {
            return obj < other.obj;
        }
        if (key != other.key) {
            return key < other.key;
        }
        return val < other.val;
    }
};

struct DeletedPropertyInfo {
    ObjectId obj;
    ObjectId key;

    DeletedPropertyInfo(ObjectId obj, ObjectId key) :
        obj(obj),
        key(key)
    { }

    bool operator<(const DeletedPropertyInfo& other) const
    {
        if (obj != other.obj) {
            return obj < other.obj;
        }
        return key < other.key;
    }
};

struct NewEdgeInfo {
    ObjectId from;
    ObjectId to;
    ObjectId type;
    ObjectId edge;

    NewEdgeInfo(ObjectId from, ObjectId to, ObjectId type, ObjectId edge) :
        from(from),
        to(to),
        type(type),
        edge(edge)
    { }

    bool operator<(const NewEdgeInfo& other) const
    {
        if (from != other.from) {
            return from < other.from;
        }
        if (to != other.to) {
            return to < other.to;
        }
        if (type != other.type) {
            return type < other.type;
        }
        return edge < other.edge;
    }
};

struct EditEdgeTypeInfo {
    ObjectId type;
    ObjectId edge;

    EditEdgeTypeInfo( ObjectId type, ObjectId edge) :
        type(type),
        edge(edge)
    { }

    bool operator<(const NewEdgeInfo& other) const
    {
        if (type != other.type) {
            return type < other.type;
        }
        return edge < other.edge;
    }
};

class UpdateContext {
public:
    std::set<ObjectId> new_nodes;

    std::set<LabelInfo> new_labels;

    std::set<NewPropertyInfo> new_properties;

    std::set<NewEdgeInfo> new_edges;

    std::set<EditEdgeTypeInfo> edit_edge_type;

    std::set<DeleteObjectInfo> deleted_objects;

    std::set<LabelInfo> deleted_labels;

    std::set<DeletedPropertyInfo> deleted_properties;

    uint64_t current_anon;

    uint64_t current_edge;

    UpdateContext()
    {
        current_anon = quad_model.catalog.max_anon;
        current_edge = quad_model.catalog.max_edge;
    }

    void insert_node(ObjectId node)
    {
        new_nodes.insert(node);
    }

    void insert_label(ObjectId node, ObjectId label)
    {
        new_labels.emplace(node, label);
    }

    void insert_property(ObjectId obj, ObjectId key, ObjectId val)
    {
        new_properties.emplace(obj, key, val);
    }

    void insert_edge(ObjectId from, ObjectId to, ObjectId type, ObjectId edge)
    {
        insert_node(from);
        insert_node(to);
        insert_node(type);
        new_edges.emplace(from, to, type, edge);
    }

    ObjectId get_new_edge_id()
    {
        return ObjectId(ObjectId::MASK_EDGE | current_edge++);
    }

    ObjectId get_anon_id()
    {
        return ObjectId(ObjectId::MASK_ANON_INLINED | current_edge++);
    }

    void delete_object(ObjectId oid, bool detach)
    {
        if (oid.get_type() == ObjectId::MASK_EDGE) {
        } else {
            deleted_objects.emplace(oid, detach);
        }
    }

    void delete_label();
    void delete_property();
    // void delete_edge();
};
} // namespace MQL