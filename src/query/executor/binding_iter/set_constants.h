#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"

class SetConstants : public BindingIter {
public:
    SetConstants(std::unique_ptr<BindingIter>&& iter, std::set<std::pair<VarId, ObjectId>> var2oid) :
        child_iter(std::move(iter)),
        var2oid(var2oid)
    { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> child_iter;

    std::set<std::pair<VarId, ObjectId>> var2oid;
private:
    Binding* parent_binding;
};
