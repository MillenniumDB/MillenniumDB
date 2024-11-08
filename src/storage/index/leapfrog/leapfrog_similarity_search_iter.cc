#include "leapfrog_similarity_search_iter.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include "graph_models/common/conversions.h"
#include "storage/index/tensor_store/tensor_store.h"


LeapfrogSimilaritySearchIter::LeapfrogSimilaritySearchIter(bool* interruption_requested,
                                                           std::vector<std::unique_ptr<ScanRange>> initial_ranges,
                                                           std::vector<VarId>                      intersection_vars,
                                                           std::vector<VarId>                      enumeration_vars,
                                                           const std::vector<float>&               query_tensor,
                                                           int64_t                                 k,
                                                           int64_t                                 search_k,
                                                           const TensorStore&                      tensor_store) :
    LeapfrogIter(interruption_requested,
                 std::move(initial_ranges),
                 std::move(intersection_vars),
                 std::move(enumeration_vars)) {

    const auto top_k = tensor_store.query_top_k(query_tensor, k, search_k);
    std::transform(top_k.begin(), top_k.end(), std::back_inserter(sorted_top_k), [](auto& pair) {
        return std::make_pair(pair.first, Common::Conversions::pack_float(pair.second).id);
    });
    std::sort(sorted_top_k.begin(), sorted_top_k.end(), [](auto& pair1, auto& pair2) {
        return pair1.first < pair2.first;
    });
}


void LeapfrogSimilaritySearchIter::down() {
    level++;
}


bool LeapfrogSimilaritySearchIter::next() {
    if (level == 0)
        return ++current != sorted_top_k.end();
    return false;
}


bool LeapfrogSimilaritySearchIter::seek(uint64_t key) {
    if (level == 0) {
        auto candidate = std::upper_bound(sorted_top_k.begin(), sorted_top_k.end(), key, [](uint64_t key, auto& pair) {
            return key < pair.first;
        });
        if (candidate != sorted_top_k.end()) {
            current = candidate;
            return true;
        }
    }
    return false;
}


bool LeapfrogSimilaritySearchIter::open_terms(Binding& /*input_binding*/) {
    level   = -1;
    current = sorted_top_k.begin();
    return true;
}


void LeapfrogSimilaritySearchIter::begin_enumeration() {
    similarity_was_enum = false;
    saved_similarity    = current->second;
}


void LeapfrogSimilaritySearchIter::reset_enumeration() {
    similarity_was_enum = false;
}


bool LeapfrogSimilaritySearchIter::next_enumeration(Binding& binding) {
    assert(level == 0 && "level must be 0");
    if (similarity_was_enum) {
        return false;
    } else {
        similarity_was_enum = true;
        binding.add(enumeration_vars[0], ObjectId(saved_similarity));
        return true;
    }
}


bool LeapfrogSimilaritySearchIter::try_estimate(std::vector<double>& initial_estimations, std::vector<double>& after_estimations) const {
    initial_estimations.push_back(sorted_top_k.size());
    after_estimations.push_back(current - sorted_top_k.begin());

    return true;
}
