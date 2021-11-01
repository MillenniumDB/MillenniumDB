#ifndef BASE__PROPERTY_PATH_PARSER_H
#define BASE__PROPERTY_PATH_PARSER_H

#include "base/parser/grammar/query/query_ast.h"
#include "base/parser/logical_plan/op/op_path.h"

/*
PathConstructor takes the a object that represents a property path
according to the grammar and transform it in a OpPath
*/

class PathConstructor {
public:
    PathConstructor() = default;
    ~PathConstructor() = default;

    std::unique_ptr<OpPath> operator()(query::ast::PropertyPathAlternatives& p , bool inverse = false);
    std::unique_ptr<OpPath> operator()(query::ast::PropertyPathSequence& p, bool inverse = false);
    std::unique_ptr<OpPath> operator()(query::ast::PropertyPathAtom& p, bool inverse = false);
};

#endif // BASE__PROPERTY_PATH_PARSER_H