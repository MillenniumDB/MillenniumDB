#pragma once

#include <memory>
#include <set>

#include "query/exceptions.h"
#include "query/executor/binding.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/id.h"
#include "query/parser/expr/mql/expr.h"
#include "query/parser/expr/mql/expr_printer.h"
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

    virtual void print(std::ostream& os, int indent) const = 0;

    // to check used vars are declared
    virtual std::set<VarId> get_input_vars() const = 0;

protected:
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

        if (node_.is_null()) {
            throw QueryExecutionException("cannot insert a null node");
        }

        if (node_.get_type() == ObjectId::MASK_EDGE) {
            return;
        }

        auto node_id = transform_if_tmp(node_).id;
        ctx.insert_node(node_id);
    }

    void print(std::ostream& os, int indent) const override
    {
        os << std::string(indent, ' ') << "InsertNode(" << node << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        if (node.is_var()) {
            res.insert(node.get_var());
        }
        return res;
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

        if (node_.is_null()) {
            throw QueryExecutionException("cannot insert label to a null node");
        }

        if (label.is_null()) {
            throw QueryExecutionException("cannot set label null label");
        }

        if (node_.get_type() == ObjectId::MASK_EDGE) {
            throw QueryExecutionException("cannot insert label to an edge");
        }

        auto node_id = transform_if_tmp(node_).id;
        auto label_id = transform_if_tmp(label).id;

        ctx.insert_label(node_id, label_id);
    }

    void print(std::ostream& os, int indent) const override
    {
        os << std::string(indent, ' ') << "InsertLabel(" << node << "," << label << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        if (node.is_var()) {
            res.insert(node.get_var());
        }
        return res;
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

        if (obj_.is_null()) {
            throw QueryExecutionException("cannot insert label to a null node");
        }

        if (label.is_null()) {
            throw QueryExecutionException("cannot set label null label");
        }

        auto obj_id = transform_if_tmp(obj_).id;
        auto label_id = transform_if_tmp(label).id;
        if (obj_.get_type() == ObjectId::MASK_EDGE) {
            ctx.set_edge_type(obj_id, label_id);
        } else {
            ctx.insert_label(obj_id, label_id);
        }
    }

    void print(std::ostream& os, int indent) const override
    {
        os << std::string(indent, ' ') << "SetLabelOrType(" << obj << "," << label << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        if (obj.is_var()) {
            res.insert(obj.get_var());
        }
        return res;
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
            throw QueryExecutionException("cannot create a property with null key or object");
        }

        // TODO: validate val_id is not a node/edge/path?
        if (val_.is_null()) {
            throw QueryExecutionException("cannot create a property with null value");
        }

        auto obj_id = transform_if_tmp(obj_).id;
        auto key_id = transform_if_tmp(key).id;
        auto value_id = transform_if_tmp(val_).id;
        ctx.insert_property(obj_id, key_id, value_id);
    }

    void print(std::ostream& os, int indent) const override
    {
        os << std::string(indent, ' ') << "InsertProperty(" << obj << "," << key << "," << val << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        if (obj.is_var()) {
            res.insert(obj.get_var());
        }
        if (val.is_var()) {
            res.insert(val.get_var());
        }
        return res;
    }
};

class InsertPropertyExpr : public UpdateAction {
public:
    Id obj;
    ObjectId key;
    std::unique_ptr<Expr> value;
    std::unique_ptr<BindingExpr> binding_expr;

    InsertPropertyExpr(Id obj, ObjectId key, std::unique_ptr<Expr> value) :
        obj(obj),
        key(key),
        value(std::move(value))
    { }

    void process(Binding& binding, UpdateContext& ctx) override
    {
        assert(binding_expr != nullptr);
        auto val_ = binding_expr->eval(binding);

        // TODO: validate val_id is not a node/edge/path?
        if (val_.is_null()) {
            throw QueryExecutionException("cannot create a property with null value");
        }

        ObjectId obj_ = obj.is_var() ? binding[obj.get_var()] : obj.get_OID();
        auto obj_id = transform_if_tmp(obj_).id;
        auto key_id = transform_if_tmp(key).id;
        auto value_id = transform_if_tmp(val_).id;
        ctx.insert_property(obj_id, key_id, value_id);
    }

    void print(std::ostream& os, int indent) const override
    {
        os << std::string(indent, ' ') << "InsertProperty(" << obj << "," << key << ",";
        ExprPrinter printer(os);
        value->accept_visitor(printer);
        os << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        if (obj.is_var()) {
            res.insert(obj.get_var());
        }
        for (auto& v : value->get_input_vars()) {
            res.insert(v);
        }
        return res;
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
            return;
        }

        auto obj_id = transform_if_tmp(obj_).id;
        auto key_id = transform_if_tmp(key).id;
        ctx.delete_property(obj_id, key_id);
    }

    void print(std::ostream& os, int indent) const override
    {
        os << std::string(indent, ' ') << "DeleteProperty(" << obj << "," << key << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        if (obj.is_var()) {
            res.insert(obj.get_var());
        }
        return res;
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
            return;
        }

        auto node_id = transform_if_tmp(node_).id;
        auto label_id = transform_if_tmp(label).id;

        ctx.delete_label(node_id, label_id);
    }

    void print(std::ostream& os, int indent) const override
    {
        os << std::string(indent, ' ') << "DeleteLabel(" << node << "," << label << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        if (node.is_var()) {
            res.insert(node.get_var());
        }
        return res;
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
            throw QueryExecutionException("cannot create an edge using a null node");
        }

        auto edge_id = ctx.get_new_edge_id().id;
        binding.add(edge, ObjectId(edge_id));

        auto from_id = transform_if_tmp(from_).id;
        auto to_id = transform_if_tmp(to_).id;
        auto type_id = transform_if_tmp(type).id;

        ctx.insert_edge(from_id, to_id, type_id, edge_id);
    }

    void print(std::ostream& os, int indent) const override
    {
        os << std::string(indent, ' ') << "InsertEdge(" << from << "," << to << "," << type << "," << edge
           << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        if (from.is_var()) {
            res.insert(from.get_var());
        }
        if (to.is_var()) {
            res.insert(to.get_var());
        }
        return res;
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
            return;
        }
        auto obj_id = transform_if_tmp(obj_).id;

        ctx.delete_object(obj_id, detach);
    }

    void print(std::ostream& os, int indent) const override
    {
        os << std::string(indent, ' ') << "DeleteObject(" << obj
           << ", DETACH: " << (detach ? "true" : "false") << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        if (obj.is_var()) {
            res.insert(obj.get_var());
        }
        return res;
    }
};

class CreateTextIndex : public UpdateAction {
public:
    const std::string index_name;
    const std::string property;
    const TextSearch::NORMALIZE_TYPE normalize_type;
    const TextSearch::TOKENIZE_TYPE tokenize_type;

    CreateTextIndex(
        std::string&& index_name,
        std::string&& property,
        TextSearch::NORMALIZE_TYPE normalize_type,
        TextSearch::TOKENIZE_TYPE tokenize_type
    ) :
        index_name(std::move(index_name)),
        property(std::move(property)),
        normalize_type(normalize_type),
        tokenize_type(tokenize_type)
    { }

    void process(Binding&, UpdateContext& ctx) override
    {
        ctx.create_text_index(*this);
    }

    void print(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpCreateTextIndex(index_name: " << index_name << ", property: " << property
           << ", normalize_type: " << normalize_type << ", tokenize_type: " << tokenize_type << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        return res;
    }
};

class CreateHNSWIndex : public UpdateAction {
public:
    const std::string index_name;
    const std::string property;
    const uint64_t dimension;
    const uint64_t max_edges;
    const uint64_t max_candidates;
    const HNSW::MetricType metric_type;

    CreateHNSWIndex(
        std::string&& index_name,
        std::string&& property,
        uint64_t dimension,
        uint64_t num_edges,
        uint64_t num_candidates,
        HNSW::MetricType metric_type
    ) :
        index_name(std::move(index_name)),
        property(std::move(property)),
        dimension(dimension),
        max_edges(num_edges),
        max_candidates(num_candidates),
        metric_type(metric_type)
    { }

    void process(Binding&, UpdateContext& ctx) override
    {
        ctx.create_hnsw_index(*this);
    }

    void print(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpCreateHNSWIndex(index_name: " << index_name << ", property: " << property
           << ", dimension: " << dimension << ", num_edges: " << max_edges
           << ", num_candidates: " << max_candidates << ", metric_type: " << metric_type << ")\n";
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        return res;
    }
};

} // namespace MQL
