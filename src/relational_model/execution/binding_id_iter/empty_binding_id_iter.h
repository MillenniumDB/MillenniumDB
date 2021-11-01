#ifndef RELATIONAL_MODEL__EMPTY_BINDING_ID_ITER_H_
#define RELATIONAL_MODEL__EMPTY_BINDING_ID_ITER_H_

#include "base/binding/binding_id_iter.h"

class EmptyBindingIdIter : public BindingIdIter {
public:
    EmptyBindingIdIter() = default;
    ~EmptyBindingIdIter() = default;

    void analyze(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "EmptyBindingIdIter()";
    }

    void begin(BindingId&) override { }
    bool next() override { return false; }
    void reset() override { }
    void assign_nulls() override { }

    static EmptyBindingIdIter instance;
};

#endif // RELATIONAL_MODEL__EMPTY_BINDING_ID_ITER_H_
