#pragma once
/*
 * LeapfrogSimilaritySearchIter is built with the result of a top-k similarity search query. In order to be able to work
 * with the Leapfrog algorithm the result is sorted by ObjectId instead of the similarity metric.
 */

#include "storage/index/leapfrog/leapfrog_iter.h"

class TensorStore;

class LeapfrogSimilaritySearchIter : public LeapfrogIter {
public:
    // Pairs { object_id, similarity } sorted by the first element
    std::vector<std::pair<uint64_t, uint64_t>> sorted_top_k;

    std::vector<std::pair<uint64_t, uint64_t>>::iterator current;

    LeapfrogSimilaritySearchIter(bool*                                   interruption_requested,
                                 std::vector<std::unique_ptr<ScanRange>> initial_ranges,
                                 std::vector<VarId>                      intersection_vars,
                                 std::vector<VarId>                      enumeration_vars,
                                 const std::vector<float>&               query_tensor,
                                 int64_t                                 k,
                                 int64_t                                 search_k,
                                 const TensorStore&                      tensor_store);

    inline uint64_t get_key() const override { return (level == 0) ? current->first : current->second; }

    void down() override;

    bool next() override;

    bool seek(uint64_t key) override;

    bool open_terms(Binding& input_binding) override;

    void begin_enumeration() override;

    void reset_enumeration() override;

    bool next_enumeration(Binding& binding) override;

    std::string get_iter_name() const override { return "LeapfrogSimilaritySearchIter"; }

    bool try_estimate(std::vector<double>& initial_estimations, std::vector<double>& after_estimations) const override;

private:

    // Whether the enumeration of the similarity was done
    bool similarity_was_enum;

    uint64_t saved_similarity;
};