#pragma once

#include <memory>

#include "query/executor/binding.h"
#include "query/id.h"
#include "query/parser/expr/mql/expr.h"
#include "query/update/mql/update_context.h"

namespace MQL {

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
        // TODO: validate var is not edge?
        ctx.insert_label(binding[var], label);
    }
};

class SetLabelOrType : public UpdateAction {
public:
    VarId var;
    ObjectId label;

    SetLabelOrType(VarId var, ObjectId label) :
        var(var),
        label(label)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        // TODO: if edge manage different case
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

    void process(Binding&, UpdateContext& ctx) override
    {
        ctx.delete_object(oid, detach);
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
        ctx.delete_object(binding[var], detach);
    }
};

} // namespace MQL
