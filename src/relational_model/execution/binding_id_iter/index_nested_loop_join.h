#ifndef RELATIONAL_MODEL__INDEX_NESTED_LOOP_JOIN_H_
#define RELATIONAL_MODEL__INDEX_NESTED_LOOP_JOIN_H_

#include <memory>
#include <vector>

#include "base/ids/var_id.h"
#include "base/binding/binding_id_iter.h"

class IndexNestedLoopJoin : public BindingIdIter {
public:
    IndexNestedLoopJoin(std::unique_ptr<BindingIdIter> lhs,
                        std::unique_ptr<BindingIdIter> rhs);
    ~IndexNestedLoopJoin() = default;

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    std::unique_ptr<BindingIdIter> lhs;
    std::unique_ptr<BindingIdIter> original_rhs;

    BindingIdIter* rhs; // will point to original_rhs or a EmptyBindingIdIter

    BindingId* parent_binding;
};

#endif // RELATIONAL_MODEL__INDEX_NESTED_LOOP_JOIN_H_
