#pragma once

#include <array>
#include <memory>

#include "base/binding/binding_id_iter.h"
#include "base/thread/thread_info.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "execution/binding_id_iter/scan_ranges/scan_range.h"

template <std::size_t N>
class IndexScan : public BindingIdIter {
private:
    BPlusTree<N>& bpt;
    ThreadInfo* thread_info;
    std::unique_ptr<BptIter<N>> it;

    BindingId* parent_binding;
    std::array<std::unique_ptr<ScanRange>, N> ranges;

    // statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches = 0;

public:
    IndexScan(BPlusTree<N>& bpt, ThreadInfo*, std::array<std::unique_ptr<ScanRange>, N> ranges);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;
};
