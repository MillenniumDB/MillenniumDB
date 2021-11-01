#ifndef BASE__VALUE_VISITOR_H_
#define BASE__VALUE_VISITOR_H_

#include <boost/variant.hpp>

class ValueVisitor : public boost::static_visitor<GraphObject> {
public:
    GraphObject operator() (common::ast::Value& value) const {
        return boost::apply_visitor(*this, value);
    }

    GraphObject operator() (int64_t const& i) const {
        return GraphObject::make_int(i);
    }

    GraphObject operator() (float const& f) const {
        return GraphObject::make_float(f);
    }

    GraphObject operator() (bool const& b) const {
        return GraphObject::make_bool(b);
    }

    GraphObject operator() (std::string const& str) const {
        // Warning: after str is destructed outside this function, the returned GraphObject will be invalid
        return GraphObject::make_string(str.c_str());
    }
};

#endif // BASE__VALUE_VISITOR_H_
