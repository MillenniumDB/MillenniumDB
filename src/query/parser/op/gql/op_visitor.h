#pragma once

#include "query/exceptions.h"

namespace GQL {
class OpGraphPattern;
class OpBasicGraphPattern;
class OpGraphPatternList;
class OpNode;
class OpWhere;
class OpEdge;
class OpReturn;
class OpPathUnion;
class OpRepetition;
class OpLinearPattern;
class OpFilter;
class OpLet;
class OpOrderBy;
class OpQueryStatements;
class OpGroupBy;
class OpUnitTable;
class OpEmpty;

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    virtual void visit(OpGraphPattern&) = 0;
    virtual void visit(OpBasicGraphPattern&) = 0;
    virtual void visit(OpGraphPatternList&) = 0;
    virtual void visit(OpNode&) = 0;
    virtual void visit(OpEdge&) = 0;
    virtual void visit(OpWhere&) = 0;
    virtual void visit(OpReturn&) = 0;
    virtual void visit(OpPathUnion&) = 0;
    virtual void visit(OpRepetition&) = 0;
    virtual void visit(OpLinearPattern&) = 0;
    virtual void visit(OpFilter&) = 0;
    virtual void visit(OpLet&) = 0;
    virtual void visit(OpOrderBy&) = 0;
    virtual void visit(OpQueryStatements&) = 0;
    virtual void visit(OpGroupBy&) = 0;
    virtual void visit(OpUnitTable&) = 0;
    virtual void visit(OpEmpty&) = 0;
};
} // namespace GQL
