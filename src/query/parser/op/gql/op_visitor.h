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
class OpNodeLabel;
class OpEdgeLabel;
class OpOptProperties;
class OpProperty;
class OpOptLabels;
class OpOrderBy;
class OpLinearPattern;
class OpFilterStatement;
class OpLet;
class OpOrderByStatement;
class OpQueryStatements;

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    virtual void visit(OpGraphPattern&)
    {
        throw LogicException("visit GQL::OpGraphPattern not implemented");
    }
    virtual void visit(OpBasicGraphPattern&)
    {
        throw LogicException("visit GQL::OpBasicGraphPattern not implemented");
    }
    virtual void visit(OpGraphPatternList&)
    {
        throw LogicException("visit GQL::OpGraphPatternList not implemented");
    }
    virtual void visit(OpNode&)
    {
        throw LogicException("visit GQL::OpNode not implemented");
    }
    virtual void visit(OpEdge&)
    {
        throw LogicException("visit GQL::OpEdge not implemented");
    }
    virtual void visit(OpFilter&)
    {
        throw LogicException("visit GQL::OpFilter not implemented");
    }
    virtual void visit(OpReturn&)
    {
        throw LogicException("visit GQL::OpReturn not implemented");
    }
    virtual void visit(OpPathUnion&)
    {
        throw LogicException("visit GQL::OpPathUnion not implemented");
    }
    virtual void visit(OpRepetition&)
    {
        throw LogicException("visit GQL::OpRepetition not implemented");
    }
    virtual void visit(OpNodeLabel&)
    {
        throw LogicException("visit GQL::OpNodeLabel not implemented");
    }
    virtual void visit(OpEdgeLabel&)
    {
        throw LogicException("visit GQL::OpEdgeLabel not implemented");
    }
    virtual void visit(OpOptProperties&)
    {
        throw LogicException("visit GQL::OpOptionalProperties not implemented");
    }
    virtual void visit(OpOptLabels&)
    {
        throw LogicException("visit GQL::OpOptLabels not implemented");
    }
    virtual void visit(OpProperty&)
    {
        throw LogicException("visit GQL::OpProperty not implemented");
    }
    virtual void visit(OpOrderBy&)
    {
        throw LogicException("visit GQL::OpOrderBy not implemented");
    }
    virtual void visit(OpLinearPattern&)
    {
        throw LogicException("visit GQL::OpLinearPattern not implemented");
    }
    virtual void visit(OpFilterStatement&)
    {
        throw LogicException("visit GQL::OpFilterStatement not implemented");
    }
    virtual void visit(OpLet&)
    {
        throw LogicException("visit GQL::OpLet not implemented");
    }
    virtual void visit(OpOrderByStatement&)
    {
        throw LogicException("visit GQL::OpOrderByStatement not implemented");
    }
    virtual void visit(OpQueryStatements&)
    {
        throw LogicException("visit GQL::OpQueryStatements not implemented");
    }
};
} // namespace GQL
