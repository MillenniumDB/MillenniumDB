#pragma once

#include "query/executor/binding_iter.h"
#include "storage/index/bplus_tree/bplus_tree.h"

template<std::size_t N>
class EdgeLookup : public BindingIter {
public:
    struct IdAssigned {
        Id id;
        bool assigned;

        IdAssigned(Id id, bool assigned) :
            id(id),
            assigned(assigned)
        { }
    };

    EdgeLookup(BPlusTree<N>& edge_bpt, Id edge, std::vector<IdAssigned>&& id_assigned_info);

    void print(std::ostream& os, int indent, bool stats) const override;

    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    BPlusTree<N>& edge_bpt;
    BptIter<N> it;

    Id edge;

    std::vector<IdAssigned> id_assigned_info;

    Binding* parent_binding;
};
