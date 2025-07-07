#include "hnsw_query_iterator.h"

#include "macros/likely.h"
#include "query/exceptions.h"
#include "storage/index/hnsw/hnsw_index.h"

using namespace HNSW;

HNSWQueryIterator::HNSWQueryIterator(
    const bool* interruption_requested_,
    HNSWIndex* index_,
    tensor::Tensor<float>&& query_tensor_,
    uint64_t num_neighbors_,
    uint64_t num_candidates_
) :
    index { index_ },
    interruption_requested { interruption_requested_ },
    query_tensor { std::move(query_tensor_) },
    num_neighbors { num_neighbors_ },
    num_candidates { num_candidates_ }
{
    assert(num_neighbors > 0);
}

void HNSWQueryIterator::begin()
{
    current_batch_heap = index->query(query_tensor, num_neighbors, num_candidates, &visited_set, &discarded_heap);
}

void HNSWQueryIterator::reset()
{
    visited_set.clear();
    discarded_heap.clear();
    current_batch_heap.clear();
    begin();
}

bool HNSWQueryIterator::next()
{
    if (MDB_unlikely(*interruption_requested)) {
        throw InterruptedException();
    }

    if (current_batch_heap.empty()) {
        if (discarded_heap.empty()) {
            return false;
        }
        get_next_batch();
    }

    if (!current_batch_heap.empty()) {
        current = current_batch_heap.get_min();
        current_batch_heap.pop_min();
        return true;
    }

    return false;
}

void HNSWQueryIterator::get_next_batch()
{
    assert(!discarded_heap.empty());
    assert(current_batch_heap.empty());

    // use the best (up to num_neighbors) entry points from the discarded ones
    std::vector<Entry> new_eps = discarded_heap.extract_n_min(num_neighbors);

    // update current batch
    current_batch_heap = index->resume_query(query_tensor, num_neighbors, new_eps, &visited_set, &discarded_heap);
}
