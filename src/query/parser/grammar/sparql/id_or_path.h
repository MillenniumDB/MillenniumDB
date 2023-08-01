#pragma once

#include <cassert>
#include <memory>
#include <variant>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "query/parser/paths/regular_path_expr.h"

class IdOrPath {
private:
    std::variant<VarId, ObjectId, std::unique_ptr<RegularPathExpr>> value;

public:
    IdOrPath(ObjectId iid) :
             value (iid) { }

    IdOrPath(VarId var) :
        value (var) { }

    IdOrPath(Id id) :
        value(ObjectId::get_null())
    {
        if (id.is_var())
            value = id.get_var();
        else
            value = id.get_OID();
    }

    IdOrPath(std::unique_ptr<RegularPathExpr> path) :
        value (std::move(path)) { }

    IdOrPath(IdOrPath&& other) :
        value (std::move(other.value)) { }

    IdOrPath clone() {
        if (is_path()) {
            return IdOrPath(get_path());
        } else if (is_var()) {
            return IdOrPath(get_var());
        } else {
            return IdOrPath(get_OID());
        }
    }

    void operator=(ObjectId iid) {
        value = iid;
    }

    void operator=(VarId var) {
        value = var;
    }

    void operator=(std::unique_ptr<RegularPathExpr> path) {
        value = std::move(path);
    }

    void operator=(Id id) {
        if (id.is_var())
            value = id.get_var();
        else
            value = id.get_OID();
    }

    bool is_var() const {
        return std::holds_alternative<VarId>(value);
    }

    bool is_path() const {
        return std::holds_alternative<std::unique_ptr<RegularPathExpr>>(value);
    }

    Id get_ID() {
        assert(!is_path());
        if (is_var()) {
            return Id(get_var());
        } else {
            return Id(get_OID());
        }
    }

    ObjectId get_OID() {
        return std::get<ObjectId>(value);
    }

    VarId get_var() {
        return std::get<VarId>(value);
    }

    std::unique_ptr<RegularPathExpr> get_path() const {
        return std::get<std::unique_ptr<RegularPathExpr>>(value)->clone();
    }
};
