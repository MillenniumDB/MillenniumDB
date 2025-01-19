#include "set_labels.h"

#include "graph_models/gql/conversions.h"
#include "graph_models/gql/gql_model.h"
#include "storage/index/bplus_tree/bplus_tree.h"

void SetLabels::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
}

bool SetLabels::_next()
{
    if (!child_iter->next()) {
        return false;
    }

    bool interruption = false;

    for (auto& opt_label : opt_labels) {
        BptIter<2> it;
        uint64_t object_id = (*parent_binding)[opt_label.object].id;

        if (opt_label.type == VarType::Node) {
            it = gql_model.node_label->get_range(&interruption, { object_id, 0 }, { object_id, UINT64_MAX });
        } else {
            it = gql_model.edge_label->get_range(&interruption, { object_id, 0 }, { object_id, UINT64_MAX });
        }

        std::vector<ObjectId> labels;

        auto obj_label = it.next();
        while (obj_label != nullptr) {
            ObjectId label_oid((*obj_label)[1]);
            labels.push_back(label_oid);
            obj_label = it.next();
        }

        ObjectId list_oid = GQL::Conversions::pack_list(labels);

        parent_binding->add(opt_label.labels_var, list_oid);
    }
    return true;
}

void SetLabels::_reset()
{
    child_iter->reset();
}

void SetLabels::assign_nulls()
{
    child_iter->assign_nulls();
}

void SetLabels::accept_visitor(BindingIterVisitor& visitor)
{
    visitor.visit(*this);
}
