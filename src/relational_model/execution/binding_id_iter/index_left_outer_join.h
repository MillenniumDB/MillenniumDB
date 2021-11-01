#ifndef RELATIONAL_MODEL__INDEX_LEFT_OUTER__JOIN_H_
#define RELATIONAL_MODEL__INDEX_LEFT_OUTER__JOIN_H_

#include <memory>
#include <vector>

#include "base/ids/var_id.h"
#include "base/binding/binding_id_iter.h"

class IndexLeftOuterJoin : public BindingIdIter {
public:
    IndexLeftOuterJoin(std::unique_ptr<BindingIdIter> lhs,
                       std::unique_ptr<BindingIdIter> rhs);
    ~IndexLeftOuterJoin() = default;

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

    // true if next rhs->next() == false has to return with nulls at the right
    bool must_return_null;
    uint_fast32_t results_found = 0;

private:
    std::unique_ptr<BindingIdIter> lhs;
    std::unique_ptr<BindingIdIter> original_rhs;

    BindingIdIter* rhs; // will point to original_rhs or a EmptyBindingIdIter

    BindingId* parent_binding;
};

#endif // RELATIONAL_MODEL__INDEX_LEFT_OUTER__JOIN_H_
