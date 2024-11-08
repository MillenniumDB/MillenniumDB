#include "index_scan.h"

#include <cassert>
#include <vector>

#include "query/var_id.h"
#include "storage/index/record.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"

template <std::size_t N>
void IndexScan<N>::_begin(Binding& parent_binding) {
    assert(ranges.size() == N && "Inconsistent size of ranges and bpt");

    this->parent_binding = &parent_binding;
    std::array<uint64_t, N> min_ids;
    std::array<uint64_t, N> max_ids;

    for (uint_fast32_t i = 0; i < N; ++i) {
        assert(ranges[i] != nullptr);
        min_ids[i] = ranges[i]->get_min(parent_binding);
        max_ids[i] = ranges[i]->get_max(parent_binding);
    }

    it = bpt.get_range(
        &get_query_ctx().thread_info.interruption_requested,
        Record<N>(std::move(min_ids)),
        Record<N>(std::move(max_ids))
    );
    ++bpt_searches;
}


template <std::size_t N>
bool IndexScan<N>::_next() {
    auto next = it.next();
    if (next != nullptr) {
        for (uint_fast32_t i = 0; i < N; ++i) {
            ranges[i]->try_assign(*parent_binding, ObjectId((*next)[i]));
        }
        return true;
    } else {
        return false;
    }
}


template <std::size_t N>
void IndexScan<N>::_reset() {
    std::array<uint64_t, N> min_ids;
    std::array<uint64_t, N> max_ids;

    for (uint_fast32_t i = 0; i < N; ++i) {
        min_ids[i] = ranges[i]->get_min(*parent_binding);
        max_ids[i] = ranges[i]->get_max(*parent_binding);
    }

    it = bpt.get_range(
        &get_query_ctx().thread_info.interruption_requested,
        Record<N>(std::move(min_ids)),
        Record<N>(std::move(max_ids))
    );
    ++bpt_searches;
}


template <std::size_t N>
void IndexScan<N>::assign_nulls() {
    for (uint_fast32_t i = 0; i < N; ++i) {
        ranges[i]->try_assign(*parent_binding, ObjectId::get_null());
    }
}


template <std::size_t N>
void IndexScan<N>::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}

template class IndexScan<1>;
template class IndexScan<2>;
template class IndexScan<3>;
template class IndexScan<4>;
