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
        ranges (std::move(ranges)),
        bpt    (bpt) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    // statistics
    uint_fast32_t bpt_searches = 0;
    std::array<std::unique_ptr<ScanRange>, N> ranges;

private:
    BPlusTree<N>& bpt;
    BptIter<N> it;

    Binding* parent_binding;
};
