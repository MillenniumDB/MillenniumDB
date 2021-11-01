#ifndef RELATIONAL_MODEL__SINGLE_RESULT_BINDING_ID_ITER_H_
#define RELATIONAL_MODEL__SINGLE_RESULT_BINDING_ID_ITER_H_

#include "base/binding/binding_id_iter.h"

class SingleResultBindingIdIter : public BindingIdIter {
private:
    bool returned = false;
public:
    SingleResultBindingIdIter() = default;
    ~SingleResultBindingIdIter() = default;

    void begin(BindingId&) override { }
    void reset() override { returned = false; }
    void assign_nulls() override { }

    bool next() override {
        if (returned) {
            return false;
        } else {
            returned = true;
            return true;
        }
    }

    void analyze(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "SingleResultBindingIdIter()";
    }
};

#endif // RELATIONAL_MODEL__SINGLE_RESULT_BINDING_ID_ITER_H_
