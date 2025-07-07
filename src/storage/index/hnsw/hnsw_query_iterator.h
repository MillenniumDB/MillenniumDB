#pragma once

#include "graph_models/common/datatypes/tensor/tensor.h"
#include "storage/index/hnsw/hnsw_heap.h"
#include "storage/index/hnsw/hnsw_visited_set.h"

namespace HNSW {

class HNSWIndex;

class HNSWQueryIterator {
public:
    explicit HNSWQueryIterator(
        const bool* interruption_requested,
        HNSWIndex* index_,
        tensor::Tensor<float>&& query_tensor,
        uint64_t num_neighbors,
        uint64_t num_candidates
    );

    // must be called once and at the beginning
    void begin();

    // resets to the initial iteration state
    void reset();

    // returns true if a result was written at "current" member
    bool next();

    // get a reference to the current result
    inline const Entry& get_current() const
    {
        return current;
    }

private:
    HNSWIndex* index;
    const bool* interruption_requested;
    const tensor::Tensor<float> query_tensor;
    const uint64_t num_neighbors;
    const uint64_t num_candidates;

    HNSWVisitedSet visited_set;
    HNSWHeap discarded_heap;
    HNSWHeap current_batch_heap;

    Entry current;

    // resumes query given the visited nodes and discarded results
    void get_next_batch();
};

} // namespace HNSW
