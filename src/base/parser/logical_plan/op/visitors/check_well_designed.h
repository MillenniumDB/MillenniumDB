/**
 * Will throw an exception if query is not well designed
 */
#ifndef BASE__CHECK_WELL_DESIGNED_H_
#define BASE__CHECK_WELL_DESIGNED_H_

#include <set>

#include "base/graph/graph_object.h"
#include "base/parser/logical_plan/op/visitors/op_visitor.h"
#include "base/parser/logical_plan/var.h"

class CheckWellDesigned : public OpVisitor {
private:
    std::set<Var> parent;
    std::set<Var> global;

public:
    void visit(OpBasicGraphPattern&) override;
    void visit(OpDistinct&)          override;
    void visit(OpWhere&)            override;
    void visit(OpMatch&)  override;
    void visit(OpGroupBy&)           override;
    void visit(OpOptional&)          override;
    void visit(OpOrderBy&)           override;
    void visit(OpSelect&)            override;

    void visit(OpConnection&)       override { }
    void visit(OpIsolatedTerm&)     override { }
    void visit(OpIsolatedVar&)      override { }
    void visit(OpLabel&)            override { }
    void visit(OpPath&)             override { }
    void visit(OpPathAlternatives&) override { }
    void visit(OpPathAtom&)         override { }
    void visit(OpPathSequence&)     override { }
    void visit(OpPathKleeneStar&)   override { }
    void visit(OpPathOptional&)     override { }
    void visit(OpProperty&)         override { }
    void visit(OpPropertyPath&)     override { }
};

#endif // BASE__CHECK_WELL_DESIGNED_H_
