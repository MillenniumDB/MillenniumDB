#include "edge_enum.h"

#include <cassert>

template<std::size_t N>
EdgeEnum<N>::EdgeEnum(BPlusTree<N>& edge_bpt, VarId edge) :
    edge_bpt(edge_bpt),
    edge(edge)
{ }

template<std::size_t N>
void EdgeEnum<N>::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;

    _reset();
}

template<std::size_t N>
void EdgeEnum<N>::_reset()
{
    std::array<uint64_t, N> min_ids;
    std::array<uint64_t, N> max_ids;

    for (size_t i = 0; i < N; ++i) {
        min_ids[i] = 0;
        max_ids[i] = UINT64_MAX;
    }

    it = edge_bpt.get_range(&get_query_ctx().thread_info.interruption_requested, min_ids, max_ids);
}

template<std::size_t N>
bool EdgeEnum<N>::_next()
{
    auto record = it.next();
    if (record != nullptr) {
        parent_binding->add(edge, ObjectId((*record)[0]));
        return true;
    }
    return false;
}

template<std::size_t N>
void EdgeEnum<N>::assign_nulls()
{
    parent_binding->add(edge, ObjectId::get_null());
}

template<std::size_t N>
void EdgeEnum<N>::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "EdgeEnum(" << edge << ")\n";
}

template class EdgeEnum<4>;
