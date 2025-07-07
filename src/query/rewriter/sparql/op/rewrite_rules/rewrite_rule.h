#pragma once

#include <set>
#include <memory>

#include "query/query_context.h"
#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"

namespace SPARQL {
/**
* The abstract class for writing a rewrite rule. It can be assumed
* that is_possible_to_regroup will always be called before regroup.
*/
class RewriteRule {

public:
    virtual ~RewriteRule() = default;

    virtual bool is_possible_to_regroup(std::unique_ptr<Op>&) = 0;
    virtual std::unique_ptr<Op> regroup(std::unique_ptr<Op>) = 0;


protected:
    template<typename T>
    bool is_castable_to(std::unique_ptr<Op> &unknown_op) {
      if (dynamic_cast<T*>(unknown_op.get()) != nullptr) {
        return true;
      }
      return false;
    }

    template<typename T>
    bool is_castable_to(std::unique_ptr<Expr> &unknown_op) {
      if (dynamic_cast<T*>(unknown_op.get()) != nullptr) {
        return true;
      }
      return false;
    }

    template<typename T>
    std::unique_ptr<T> cast_to(std::unique_ptr<Op> unknown_op) {
      std::unique_ptr<T> casted_optional(
        dynamic_cast<T*>(unknown_op.release()));
      return casted_optional;
    }

    template<typename T>
    std::unique_ptr<T> cast_to(std::unique_ptr<Expr> unknown_op) {
      std::unique_ptr<T> casted_optional(
        dynamic_cast<T*>(unknown_op.release()));
      return casted_optional;
    }
};
} // namespace SPARQL
