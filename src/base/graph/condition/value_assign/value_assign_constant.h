#ifndef BASE__VALUE_ASSIGN_CONSTANT_H_
#define BASE__VALUE_ASSIGN_CONSTANT_H_

#include <memory>
#include <string>

#include "base/graph/condition/value_assign/value_assign.h"
#include "base/graph/graph_object.h"

class ValueAssignConstant : public ValueAssign {
private:
    GraphObject value;
    std::unique_ptr<std::string> str; // used to hold the string value when GraphObject is a StringExternal
                                      // otherwise the char pointer would be invalid is the str is destroyed

public:
    ValueAssignConstant(GraphObject value, std::unique_ptr<std::string> str) :
        value (value),
        str   (std::move(str)) { }

    ~ValueAssignConstant() = default;

    GraphObject get_value(Binding&) const {
        return value;
    }
};

#endif // BASE__VALUE_ASSIGN_CONSTANT_H_
