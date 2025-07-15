#include "edge_lookup.h"

#include <cassert>

template<std::size_t N>
EdgeLookup<N>::EdgeLookup(BPlusTree<N>& edge_bpt, Id edge, std::vector<IdAssigned>&& _id_assigned_info) :
    edge_bpt(edge_bpt),
    edge(edge),
    id_assigned_info(std::move(_id_assigned_info))
{
    assert(id_assigned_info.size() == N - 1);
}

template<std::size_t N>
void EdgeLookup<N>::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;

    _reset();
}

template<std::size_t N>
void EdgeLookup<N>::_reset()
{
    auto edge_id = edge.is_var() ? (*parent_binding)[edge.get_var()] : edge.get_OID();

    std::array<uint64_t, N> min_ids;
    std::array<uint64_t, N> max_ids;

    min_ids[0] = edge_id.id;
    max_ids[0] = edge_id.id;

    for (size_t i = 1; i < N; ++i) {
        min_ids[i] = 0;
        max_ids[i] = UINT64_MAX;
    }

    it = edge_bpt.get_range(&get_query_ctx().thread_info.interruption_requested, min_ids, max_ids);
}

template<std::size_t N>
bool EdgeLookup<N>::_next()
{
    auto record = it.next();
    if (record == nullptr) {
        return false;
    }
    auto check_id = [](Binding& binding, Id id, bool assigned, ObjectId obj_id) -> bool {
        if (id.is_var()) {
            if (assigned) {
                return binding[id.get_var()] == obj_id;
            } else {
                binding.add(id.get_var(), obj_id);
            }
        } else {
            auto value = id.get_OID();
            if (!value.is_null() && value != obj_id) {
                return false;
            }
        }
        return true;
    };

    size_t i = 1;
    for (auto& info : id_assigned_info) {
        if (!check_id(*parent_binding, info.id, info.assigned, ObjectId((*record)[i]))) {
            return false;
        }
        i++;
    }
    return true;
}

template<std::size_t N>
void EdgeLookup<N>::assign_nulls()
{
    // Not assigning edge to null, since the edge is supposed to be assigned
    for (auto& info : id_assigned_info) {
        if (!info.assigned) {
            assert(info.id.is_var());
            parent_binding->add(info.id.get_var(), ObjectId::get_null());
        }
    }
}

template<std::size_t N>
void EdgeLookup<N>::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "EdgeLookup(";

    os << "[" << edge << "]";

    for (auto& info : id_assigned_info) {
        if (info.assigned) {
            os << " [" << info.id << "]";
        } else {
            os << " " << info.id;
        }
    }

    os << ")\n";
}

template class EdgeLookup<3>;
template class EdgeLookup<4>;
