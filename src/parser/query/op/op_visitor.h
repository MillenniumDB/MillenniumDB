#pragma once

#include "base/exceptions.h"

namespace MDB {
class OpBasicGraphPattern;
class OpDescribe;
class OpEdge;
class OpGroupBy;
class OpInsert;
class OpIsolatedTerm;
class OpIsolatedVar;
class OpLabel;
class OpMatch;
class OpOptional;
class OpOrderBy;
class OpPath;
class OpProperty;
class OpReturn;
class OpSet;
class OpWhere;
} // namespace MDB

namespace SPARQL {
class OpOptional;
class OpSelect;
class OpTriples;
class OpTriple;
class OpWhere;
} // namespace SPARQL

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    // MillenniumDB
    virtual void visit(MDB::OpBasicGraphPattern&) { throw LogicException("visit MDB::OpBasicGraphPattern not implemented"); }
    virtual void visit(MDB::OpDescribe&)          { throw LogicException("visit MDB::OpDescribe not implemented"); }
    virtual void visit(MDB::OpEdge&)              { throw LogicException("visit MDB::OpEdge not implemented"); }
    virtual void visit(MDB::OpGroupBy&)           { throw LogicException("visit MDB::OpGroupBy not implemented"); }
    virtual void visit(MDB::OpInsert&)            { throw LogicException("visit MDB::OpInsert not implemented"); }
    virtual void visit(MDB::OpIsolatedTerm&)      { throw LogicException("visit MDB::OpIsolatedTerm not implemented"); }
    virtual void visit(MDB::OpIsolatedVar&)       { throw LogicException("visit MDB::OpIsolatedVar not implemented"); }
    virtual void visit(MDB::OpLabel&)             { throw LogicException("visit MDB::OpLabel not implemented"); }
    virtual void visit(MDB::OpMatch&)             { throw LogicException("visit MDB::OpMatch not implemented"); }
    virtual void visit(MDB::OpOptional&)          { throw LogicException("visit MDB::OpOptional not implemented"); }
    virtual void visit(MDB::OpOrderBy&)           { throw LogicException("visit MDB::OpOrderBy not implemented"); }
    virtual void visit(MDB::OpPath&)              { throw LogicException("visit MDB::OpPath not implemented"); }
    virtual void visit(MDB::OpProperty&)          { throw LogicException("visit MDB::OpProperty not implemented"); }
    virtual void visit(MDB::OpReturn&)            { throw LogicException("visit MDB::OpReturn not implemented"); }
    virtual void visit(MDB::OpSet&)               { throw LogicException("visit MDB::OpSet not implemented"); }
    virtual void visit(MDB::OpWhere&)             { throw LogicException("visit MDB::OpWhere not implemented"); }
    // SPARQL
    virtual void visit(SPARQL::OpOptional&)       { throw LogicException("visit SPARQL::OpOptional not implemented"); }
    virtual void visit(SPARQL::OpSelect&)         { throw LogicException("visit SPARQL::OpSelect not implemented"); }
    virtual void visit(SPARQL::OpTriples&)        { throw LogicException("visit SPARQL::OpTriples not implemented"); }
    virtual void visit(SPARQL::OpTriple&)         { throw LogicException("visit SPARQL::OpTriple not implemented"); }
    virtual void visit(SPARQL::OpWhere&)          { throw LogicException("visit SPARQL::OpWhere not implemented"); }
};