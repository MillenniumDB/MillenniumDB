#pragma once

#include <cassert>
#include <memory>
#include <variant>

#include "graph_models/object_id.h"
#include "query/var_id.h"

class Id {
union IdUnion {
    VarId    var;
    ObjectId iid;

    IdUnion(VarId var) : var (var) { }

    IdUnion(ObjectId iid) : iid (iid) { }
};

enum class IdType { VarId, ObjectId };

public:
    Id(ObjectId iid) :
        value (iid),
        type (IdType::ObjectId) { }

    Id(VarId var) :
        value (var),
        type (IdType::VarId) { }

    Id(const Id& other) :
        value (other.value),
        type  (other.type)
    {
        type = other.type;
        if (other.type == IdType::VarId) {
            value.var = other.value.var;
        } else {
            value.iid = other.value.iid;
        }
    }

    void operator=(const Id& other) {
        type = other.type;
        value = other.value;
    }

    void operator=(ObjectId iid) {
        value.iid = iid;
        type = IdType::ObjectId;
    }

    void operator=(VarId var) {
        value.var = var;
        type = IdType::VarId;
    }

    ObjectId get_OID() const  {
        return value.iid;
    }

    VarId get_var() const  {
        return value.var;
    }

    bool is_var() const {
        return type == IdType::VarId;
    }

    bool is_OID() const  {
        return type == IdType::ObjectId;
    }

    bool operator<(const Id& other) const {
        if (this->is_var() != other.is_var()) {
            return this->is_var();
        } else {
            if (this->is_var()) {
                return this->get_var() < other.get_var();
            } else {
                return this->get_OID() < other.get_OID();
            }
        }
    }

    bool operator==(const Id& other) const {
        if (this->is_var() != other.is_var()) {
            return false;
        } else {
            if (this->is_var()) {
                return this->get_var() == other.get_var();
            } else {
                return this->get_OID() == other.get_OID();
            }
        }
    }

    bool operator!=(const Id& other) const {
        return !(*this == other);
    }

private:
    IdUnion value;
    IdType type;
};