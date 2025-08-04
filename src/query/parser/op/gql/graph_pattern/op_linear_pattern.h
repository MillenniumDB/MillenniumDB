#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {

struct Label {
    VarId object;
    ObjectId label_id;
    VarType::Type type;

    Label(VarId id, ObjectId label_id, VarType::Type type) :
        object(id),
        label_id(label_id),
        type(type)
    { }

    bool operator<(const Label& other) const
    {
        if (object != other.object) {
            return object < other.object;
        } else if (label_id != other.label_id) {
            return label_id < other.label_id;
        }
        return type < other.type;
    }
};

struct Property {
    VarId object;
    ObjectId key;
    VarId var_with_property;
    ObjectId value;
    VarType::Type type;

    Property(VarId object, ObjectId key, VarId var_with_property, ObjectId value, VarType::Type type) :
        object(object),
        key(key),
        var_with_property(var_with_property),
        value(value),
        type(type)
    { }

    bool operator<(const Property& other) const
    {
        if (object != other.object) {
            return object < other.object;
        } else if (key != other.key) {
            return key < other.key;
        } else if (var_with_property != other.var_with_property) {
            return var_with_property < other.var_with_property;
        } else if (value != other.value) {
            return value < other.value;
        }
        return type < other.type;
    }
};

class OpLinearPattern : public Op {
public:
    std::vector<std::unique_ptr<Op>> patterns;

    std::set<Label> labels;
    std::set<Property> properties;

    OpLinearPattern() { }

    OpLinearPattern(std::vector<std::unique_ptr<Op>>&& patterns) :
        patterns(std::move(patterns))
    { }

    std::unique_ptr<Op> clone() const override
    {
        auto linear_pattern = std::make_unique<OpLinearPattern>();
        for (auto& pattern : patterns) {
            linear_pattern->add_pattern(pattern->clone());
        }
        for (auto& label : labels) {
            linear_pattern->add_label(label);
        }
        for (auto& property : properties) {
            linear_pattern->add_property(property);
        }
        return linear_pattern;
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void add_pattern(std::unique_ptr<Op>&& pattern)
    {
        patterns.push_back(std::move(pattern));
    }

    void add_label(Label label)
    {
        labels.insert(label);
    }

    void add_property(Property property)
    {
        properties.insert(property);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;

        for (auto& pattern : patterns) {
            auto pattern_vars = pattern->get_all_vars();
            res.merge(pattern_vars);
        }

        return res;
    }

    std::map<VarId, VarType> get_var_types() const override
    {
        std::map<VarId, VarType> result;

        for (auto& pattern : patterns) {
            for (auto& [var, type] : pattern->get_var_types()) {
                if (result.count(var) && result[var] != type) {
                    throw QuerySemanticException(
                        "Variable \"" + get_query_ctx().get_var_name(var) + "\" has more than one type."
                    );
                }
                result[var] = type;
            }
        }
        return result;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');

        if (labels.empty() && properties.empty()) {
            os << "OpLinearPattern()\n";
        } else {
            os << "OpLinearPattern(";

            bool first = true;
            for (auto& label : labels) {
                if (first) {
                    first = false;
                    os << "\n"
                       << std::string(indent + 2, ' ') << "labels: " << label.object << " " << label.label_id;
                } else {
                    os << ", " << label.object << " " << label.label_id;
                }
            }

            first = true;
            for (auto& property : properties) {
                if (first) {
                    first = false;
                    os << "\n"
                       << std::string(indent + 2, ' ') << "properties: " << property.var_with_property << "="
                       << property.value;
                } else {
                    os << ", " << property.var_with_property << "=" << property.value;
                }
            }
            os << "\n" << std::string(indent, ' ') << ")\n";
        }

        for (auto& pattern : patterns) {
            pattern->print_to_ostream(os, indent + 2);
        }
        os.flush();
        return os;
    }
};
} // namespace GQL
