#ifndef BASE__OP_VISITOR_H_
#define BASE__OP_VISITOR_H_

class OpSelect;
class OpBasicGraphPattern;
class OpWhere;
class OpConnection;
class OpLabel;
class OpOptional;
class OpProperty;
class OpOrderBy;
class OpGroupBy;
class OpIsolatedVar;
class OpIsolatedTerm;
class OpMatch;
class OpDistinct;
class OpPropertyPath;
class OpPath;
class OpPathAtom;
class OpPathAlternatives;
class OpPathSequence;
class OpPathKleeneStar;
class OpPathOptional;

class OpVisitor {
public:
    virtual ~OpVisitor() = default;
    virtual void visit(OpBasicGraphPattern&) = 0;
    virtual void visit(OpConnection&) = 0;
    virtual void visit(OpDistinct&) = 0;
    virtual void visit(OpWhere&) = 0;
    virtual void visit(OpMatch&) = 0;
    virtual void visit(OpGroupBy&) = 0;
    virtual void visit(OpIsolatedVar&) = 0;
    virtual void visit(OpIsolatedTerm&) = 0;
    virtual void visit(OpLabel&) = 0;
    virtual void visit(OpOptional&) = 0;
    virtual void visit(OpOrderBy&) = 0;
    virtual void visit(OpProperty&) = 0;
    virtual void visit(OpPropertyPath&) = 0;
    virtual void visit(OpPath&) = 0;
    virtual void visit(OpPathAtom&) = 0;
    virtual void visit(OpPathAlternatives&) = 0;
    virtual void visit(OpPathSequence&) = 0;
    virtual void visit(OpPathKleeneStar&) = 0;
    virtual void visit(OpPathOptional&) = 0;
    virtual void visit(OpSelect&) = 0;
};

#endif // BASE__OP_VISITOR_H_
