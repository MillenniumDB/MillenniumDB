#ifndef BASE__VALUE_ASSIGN_VARIABLE_H_
#define BASE__VALUE_ASSIGN_VARIABLE_H_

#include "base/graph/condition/value_assign/value_assign.h"
#include "base/ids/var_id.h"

class ValueAssignVariable : public ValueAssign {
private:
    VarId var_id;

public:
    ValueAssignVariable(VarId var_id) :
        var_id(var_id) { }

    ~ValueAssignVariable() = default;

    GraphObject get_value(Binding& binding) const {
        return binding[var_id];
    }
};

#endif // BASE__VALUE_ASSIGN_VARIABLE_H_
