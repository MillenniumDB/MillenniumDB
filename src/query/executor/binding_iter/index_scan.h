#pragma once

#include <array>
#include <memory>

#include "query/executor/binding_iter.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"

template <std::size_t N>
class IndexScan : public BindingIter {
public:
    IndexScan(
        BPlusTree<N>&                               bpt,
        std::array<std::unique_ptr<ScanRange>, N>&& ranges
    ) :
        bpt           (bpt),
        ranges        (std::move(ranges)) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    BPlusTree<N>& bpt;
    BptIter<N> it;

    Binding* parent_binding;
    std::array<std::unique_ptr<ScanRange>, N> ranges;

    // statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches = 0;
};
