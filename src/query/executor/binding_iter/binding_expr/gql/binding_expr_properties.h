#pragma once

#include "graph_models/gql/conversions.h"
#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"
#include "query/rewriter/gql/op/var_type.h"
#include "storage/index/bplus_tree/bplus_tree.h"

namespace GQL {
class BindingExprProperties : public BindingExpr {
public:
    VarId object_var;
    BPlusTree<3>* bpt;

    BindingExprProperties(VarId object, VarType::Type type) :
        object_var(object)
    {
        if (type == VarType::Node) {
            bpt = gql_model.node_key_value.get();
        } else {
            bpt = gql_model.edge_key_value.get();
        }
    }

    ObjectId eval(const Binding& binding) override
    {
        ObjectId object_id = binding[object_var];

        bool interruption = false;
        BptIter<3> it = bpt->get_range(
            &interruption,
            { object_id.id, 0, 0 },
            { object_id.id, UINT64_MAX, UINT64_MAX }
        );

        std::vector<ObjectId> properties;

        auto record = it.next();
        std::map<ObjectId, std::unique_ptr<DictionaryItem>> properties_map;

        while (record != nullptr) {
            ObjectId key_oid((*record)[1]);
            ObjectId value_oid((*record)[2]);

            properties_map.emplace(key_oid, std::make_unique<DictionaryLiteral>(value_oid));
            record = it.next();
        }

        auto dict = std::make_unique<Dictionary>(std::move(properties_map));
        ObjectId dict_oid = Conversions::pack_dictionary(dict);
        return dict_oid;
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>&) const override
    {
        os << "Properties(" << object_var << ")\n";
    }
};
} // namespace GQL
