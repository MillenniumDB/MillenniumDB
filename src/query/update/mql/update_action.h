#pragma once

#include <memory>

#include "query/executor/binding.h"
#include "query/id.h"
#include "query/parser/expr/mql/expr.h"
#include "query/update/mql/update_context.h"
#include "system/string_manager.h"
#include "system/tensor_manager.h"
#include "system/tmp_manager.h"

namespace MQL {

class UpdateAction {
public:
    virtual ~UpdateAction() = default;

    virtual void process(Binding& binding, UpdateContext&) = 0;

    static constexpr uint64_t CLEAR_TMP_MASK = ~(ObjectId::MOD_MASK | ObjectId::MASK_EXTERNAL_ID);

    ObjectId transform_if_tmp(ObjectId oid)
    {
        if (oid.is_tmp()) {
            const uint64_t tmp_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
            const auto& tmp_str = tmp_manager.get_str(tmp_id);

            const uint64_t gen_t = oid.id & ObjectId::GENERIC_TYPE_MASK;

            uint64_t new_external_id;
            if (gen_t == ObjectId::MASK_TENSOR) {
                new_external_id = tensor_manager.get_or_create_id(tmp_str.data(), tmp_str.size());
            } else {
                new_external_id = string_manager.get_or_create(tmp_str.data(), tmp_str.size());
            }

            oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_EXTERNAL | new_external_id;
        }

        assert(!oid.is_tmp());

        return oid;
    }
};

class InsertNode : public UpdateAction {
public:
    Id node;

    InsertNode(Id node) :
        node(node)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ObjectId node_ = node.is_var() ? binding[node.get_var()] : node.get_OID();

        if (node_.is_null() || node_.get_type() == ObjectId::MASK_EDGE) {
            // TODO: rollback and throw
        }

        auto node_id = transform_if_tmp(node_).id;
        ctx.insert_node(node_id);
    }
};

class InsertLabel : public UpdateAction {
public:
    Id node;
    ObjectId label;

    InsertLabel(Id node, ObjectId label) :
        node(node),
        label(label)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ObjectId node_ = node.is_var() ? binding[node.get_var()] : node.get_OID();

        if (node_.is_null() || node_.get_type() == ObjectId::MASK_EDGE) {
            // TODO: rollback and throw
        }

        auto node_id = transform_if_tmp(node_).id;
        auto label_id = transform_if_tmp(label).id;

        ctx.insert_label(node_id, label_id);
    }
};

class SetLabelOrType : public UpdateAction {
public:
    Id obj;
    ObjectId label;

    SetLabelOrType(Id obj, ObjectId label) :
        obj(obj),
        label(label)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ObjectId obj_ = obj.is_var() ? binding[obj.get_var()] : obj.get_OID();

        if (obj_.is_null() || label.is_null()) {
            // TODO: rollback and throw
        }

        auto obj_id = transform_if_tmp(obj_).id;
        auto label_id = transform_if_tmp(label).id;
        if (obj_.get_type() == ObjectId::MASK_EDGE) {
            ctx.set_edge_type(obj_id, label_id);
        } else {
            ctx.insert_label(obj_id, label_id);
        }
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
        ObjectId obj_ = obj.is_var() ? binding[obj.get_var()] : obj.get_OID();
        ObjectId val_ = val.is_var() ? binding[val.get_var()] : val.get_OID();

        if (obj_.is_null() || key.is_null()) {
            // TODO: rollback and throw
        }

        // TODO: validate val_id is not a node/edge/path?
        if (val_.is_null()) {
            // TODO: rollback and throw
        }

        auto obj_id = transform_if_tmp(obj_).id;
        auto key_id = transform_if_tmp(key).id;
        auto value_id = transform_if_tmp(val_).id;
        ctx.insert_property(obj_id, key_id, value_id);
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

class DeleteProperty : public UpdateAction {
public:
    Id obj;
    ObjectId key;

    DeleteProperty(Id obj, ObjectId key) :
        obj(obj),
        key(key)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ObjectId obj_ = obj.is_var() ? binding[obj.get_var()] : obj.get_OID();

        if (obj_.is_null() || key.is_null()) {
            // TODO: rollback and throw
            // TODO: maybe ignore the delete instead of failing?
        }

        auto obj_id = transform_if_tmp(obj_).id;
        auto key_id = transform_if_tmp(key).id;
        ctx.delete_property(obj_id, key_id);
    }
};

class DeleteLabel : public UpdateAction {
public:
    Id node;
    ObjectId label;

    DeleteLabel(Id node, ObjectId label) :
        node(node),
        label(label)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ObjectId node_ = node.is_var() ? binding[node.get_var()] : node.get_OID();

        if (node_.is_null() || node_.get_type() == ObjectId::MASK_EDGE) {
            // TODO: rollback and throw
            // TODO: maybe ignore the delete instead of failing?
        }

        auto node_id = transform_if_tmp(node_).id;
        auto label_id = transform_if_tmp(label).id;

        ctx.delete_label(node_id, label_id);
    }
};

class InsertEdge : public UpdateAction {
public:
    Id from;
    Id to;
    ObjectId type;
    VarId edge;

    InsertEdge(Id from, Id to, ObjectId type, VarId edge) :
        from(from),
        to(to),
        type(type),
        edge(edge)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ObjectId from_ = from.is_var() ? binding[from.get_var()] : from.get_OID();
        ObjectId to_ = to.is_var() ? binding[to.get_var()] : to.get_OID();

        if (from_.is_null() || to_.is_null() || type.is_null()) {
            // TODO: rollback and throw
        }

        auto edge_id = ctx.get_new_edge_id().id;
        binding[edge] = ObjectId(edge_id);

        auto from_id = transform_if_tmp(from_).id;
        auto to_id = transform_if_tmp(to_).id;
        auto type_id = transform_if_tmp(to_).id;

        ctx.insert_edge(from_id, to_id, type_id, edge_id);
    }
};

class DeleteObject : public UpdateAction {
public:
    Id obj;
    bool detach;

    DeleteObject(Id obj, bool detach) :
        obj(obj),
        detach(detach)
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        ObjectId obj_ = obj.is_var() ? binding[obj.get_var()] : obj.get_OID();

        if (obj_.is_null()) {
            // TODO: rollback and throw
            // TODO: maybe ignore the delete instead of failing?
        }
        auto obj_id = transform_if_tmp(obj_).id;

        ctx.delete_object(obj_id, detach);
    }
};

} // namespace MQL
