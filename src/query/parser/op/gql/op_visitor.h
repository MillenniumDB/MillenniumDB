#pragma once

#include "query/exceptions.h"

namespace GQL {
class OpGraphPattern;
class OpBasicGraphPattern;
class OpGraphPatternList;
class OpNode;
class OpFilter;
class OpEdge;
class OpReturn;
class OpPathUnion;
class OpRepetition;
class OpOrderBy;
class OpLinearPattern;
class OpFilterStatement;
class OpLet;
class OpOrderByStatement;
class OpQueryStatements;
class OpGroupBy;

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    virtual void visit(OpGraphPattern&) = 0;
    virtual void visit(OpBasicGraphPattern&) = 0;
    virtual void visit(OpGraphPatternList&) = 0;
    virtual void visit(OpNode&) = 0;
    virtual void visit(OpEdge&) = 0;
    virtual void visit(OpFilter&) = 0;
    virtual void visit(OpReturn&) = 0;
    virtual void visit(OpPathUnion&) = 0;
    virtual void visit(OpRepetition&) = 0;
    virtual void visit(OpOrderBy&) = 0;
    virtual void visit(OpLinearPattern&) = 0;
    virtual void visit(OpFilterStatement&) = 0;
    virtual void visit(OpLet&) = 0;
    virtual void visit(OpOrderByStatement&) = 0;
    virtual void visit(OpQueryStatements&) = 0;
    virtual void visit(OpGroupBy&) = 0;
};
} // namespace GQL
