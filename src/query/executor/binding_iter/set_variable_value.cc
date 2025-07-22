#include "set_variable_value.h"

#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

void SetVariableValues::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    returned = false;
}

bool SetVariableValues::_next()
{
    if (returned) {
        return false;
    }

    returned = true;
    for (auto& item : items) {
        ObjectId value = item.second->eval(*parent_binding);
        parent_binding->add(item.first, value);
    }
    return true;
}

void SetVariableValues::_reset()
{
    returned = false;
}

void SetVariableValues::assign_nulls()
{
    for (auto& item : items) {
        parent_binding->add(item.first, ObjectId::get_null());
    }
}

void SetVariableValues::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "SetVariableValues(";
    {
        BindingExprPrinter printer(os, indent, stats);
        bool first = true;
        for (auto& pair : items) {
            if (first) {
                first = false;
            } else {
                os << ", ";
            }
            os << "var:" << pair.first << " = ";
            printer.print(*pair.second);
        }
    }
    os << ")\n";
}
