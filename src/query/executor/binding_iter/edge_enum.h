#pragma once

#include "query/executor/binding_iter.h"
#include "storage/index/bplus_tree/bplus_tree.h"

/*
Enumerates all edges from a b+tree where its first component is the edge_id.
Only assigns the edge var. It supposes each record has a unique edge_id.
*/
template<std::size_t N>
class EdgeEnum : public BindingIter {
public:
    EdgeEnum(BPlusTree<N>& edge_bpt, VarId edge);

    void print(std::ostream& os, int indent, bool stats) const override;

    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    BPlusTree<N>& edge_bpt;
    BptIter<N> it;

    VarId edge;

    Binding* parent_binding;
};
