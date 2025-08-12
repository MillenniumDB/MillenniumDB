#pragma once

#include <memory>
#include <set>

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding.h"
#include "query/id.h"
#include "query/parser/expr/mql/expr.h"

namespace MQL {

struct DeleteNodeInfo {
    ObjectId node;
    bool detach;

    DeleteNodeInfo(ObjectId node, bool detach) :
        node(node),
        detach(detach)
    { }

    bool operator<(const DeleteNodeInfo& other) const
    {
        if (node != other.node) {
            return node < other.node;
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

class UpdateContext {
public:
    std::set<ObjectId> new_nodes;

    std::set<LabelInfo> new_labels;

    std::set<NewPropertyInfo> new_properties;

    std::set<NewEdgeInfo> new_edges;

    std::set<DeleteNodeInfo> deleted_nodes;

    std::set<LabelInfo> deleted_labels;

    std::set<DeletedPropertyInfo> deleted_properties;

    std::set<ObjectId> deleted_edges;

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

    void delete_node(ObjectId node, bool detach)
    {
        deleted_nodes.emplace(node, detach);
    }

    void delete_label();
    void delete_property();
    void delete_edge();
};

class UpdateAction {
public:
    virtual ~UpdateAction() = default;

    virtual void process(Binding& binding, UpdateContext&) = 0;
};

class InsertNode : public UpdateAction {
public:
    VarId var;

    InsertNode(VarId var) :
        var(var)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ctx.insert_node(binding[var]);
    }
};

class InsertLabel : public UpdateAction {
public:
    VarId var;
    ObjectId label;

    InsertLabel(VarId var, ObjectId label) :
        var(var),
        label(label)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ctx.insert_label(binding[var], label);
    }
};

class InsertProperty : public UpdateAction {
public:
    Id obj;
    ObjectId key;
    Id val;

    InsertProperty(Id obj, ObjectId key, Id val) :
        obj(obj),
        key(key),
        val(val)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ObjectId obj_id = obj.is_var() ? binding[obj.get_var()] : obj.get_OID();
        ObjectId val_id = val.is_var() ? binding[val.get_var()] : val.get_OID();
        ctx.insert_property(obj_id, key, val_id);
    }
};

class InsertPropertyExpr : public UpdateAction {
public:
    Id obj;
    ObjectId key;
    std::unique_ptr<Expr> value;

    InsertPropertyExpr(Id obj, ObjectId key, std::unique_ptr<Expr> value) :
        obj(obj),
        key(key),
        value(std::move(value))
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        // TODO: throw exception if value evaluated is null
        assert(false);
    }
};

class InsertEdge : public UpdateAction {
public:
    Id from;
    Id to;
    ObjectId type;
    ObjectId edge;

    InsertEdge(Id from, Id to, ObjectId type, ObjectId edge) :
        from(from),
        to(to),
        type(type),
        edge(edge)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ObjectId from_id = from.is_var() ? binding[from.get_var()] : from.get_OID();
        ObjectId to_id = to.is_var() ? binding[to.get_var()] : to.get_OID();
        ctx.insert_edge(from_id, to_id, type, edge);
    }
};

class DeleteObject : public UpdateAction {
public:
    ObjectId oid;
    bool detach;

    DeleteObject(ObjectId oid, bool detach) :
        oid(oid),
        detach(detach)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        // TODO:
    }
};

class DeleteVar : public UpdateAction {
public:
    VarId var;
    bool detach;

    DeleteVar(VarId var, bool detach) :
        var(var),
        detach(detach)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ctx.add_deleted(binding[var], detach);
    }
};

} // namespace MQL
