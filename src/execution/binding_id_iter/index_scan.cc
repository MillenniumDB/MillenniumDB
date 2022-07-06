#include "index_scan.h"

#include <cassert>
#include <vector>

#include "base/ids/var_id.h"
#include "storage/index/record.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"

using namespace std;

template <std::size_t N>
IndexScan<N>::IndexScan(BPlusTree<N>& bpt, ThreadInfo* thread_info, std::array<std::unique_ptr<ScanRange>, N> ranges) :
    bpt         (bpt),
    thread_info (thread_info),
    ranges      (move(ranges)) { }


template <std::size_t N>
void IndexScan<N>::begin(BindingId& parent_binding) {
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
        &thread_info->interruption_requested,
        Record<N>(std::move(min_ids)),
        Record<N>(std::move(max_ids))
    );
    ++bpt_searches;
}


template <std::size_t N>
bool IndexScan<N>::next() {
    assert(it != nullptr);
    auto next = it->next();
    if (next != nullptr) {
        for (uint_fast32_t i = 0; i < N; ++i) {
            ranges[i]->try_assign(*parent_binding, ObjectId(next->ids[i]));
        }
        ++results_found;
        return true;
    } else {
        return false;
    }
}


template <std::size_t N>
void IndexScan<N>::reset() {
    std::array<uint64_t, N> min_ids;
    std::array<uint64_t, N> max_ids;

    for (uint_fast32_t i = 0; i < N; ++i) {
        min_ids[i] = ranges[i]->get_min(*parent_binding);
        max_ids[i] = ranges[i]->get_max(*parent_binding);
    }

    it = bpt.get_range(
        &thread_info->interruption_requested,
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
void IndexScan<N>::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    auto real_factor = static_cast<double>(results_found) / static_cast<double>(bpt_searches);
    os << "IndexScan(bpt_searches: " << bpt_searches << ", found: " << results_found
       << ", factor: " << real_factor << ")";
}

template class IndexScan<1>;
template class IndexScan<2>;
template class IndexScan<3>;
template class IndexScan<4>;
