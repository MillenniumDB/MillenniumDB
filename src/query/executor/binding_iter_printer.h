#pragma once

#include <memory>
#include <ostream>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter_visitor.h"
#include "query/var_id.h"


class BindingIterPrinter : public BindingIterVisitor {
    friend class BindingIterPrinterHelper;

private:
    std::ostream& os;
    bool print_stats;
    int indent;

public:
    BindingIterPrinter(std::ostream& os, bool print_stats, int indent) :
        os(os), print_stats(print_stats), indent(indent) { }

    void print_join(const std::vector<VarId>&           parent_safe_vars,
                    const std::vector<VarId>&           lhs_only_vars,
                    const std::vector<VarId>&           safe_join_vars,
                    const std::vector<VarId>&           unsafe_join_vars,
                    const std::vector<VarId>&           rhs_only_vars,
                    const std::unique_ptr<BindingIter>& lhs,
                    const std::unique_ptr<BindingIter>& rhs);

    template<size_t N>
    void print_index_scan(IndexScan<N>& binding_iter);

    virtual void visit(Aggregation&)               override;
    virtual void visit(Bind&)                      override;
    virtual void visit(CrossProduct&)              override;
    virtual void visit(DistinctHash&)              override;
    virtual void visit(EdgeTableLookup&)           override;
    virtual void visit(EmptyBindingIter&)          override;
    virtual void visit(Filter&)                    override;
    virtual void visit(ExprEvaluator&)             override;
    virtual void visit(IndexLeftOuterJoin&)        override;
    virtual void visit(IndexNestedLoopJoin&)       override;
    virtual void visit(IndexScan<1>&)              override;
    virtual void visit(IndexScan<2>&)              override;
    virtual void visit(IndexScan<3>&)              override;
    virtual void visit(IndexScan<4>&)              override;
    virtual void visit(LeapfrogJoin&)              override;
    virtual void visit(NestedLoopAntiJoin<false>&) override;
    virtual void visit(NestedLoopAntiJoin<true>&)  override;
    virtual void visit(NestedLoopJoin&)            override;
    virtual void visit(NestedLoopLeftJoin&)        override;
    virtual void visit(NestedLoopSemiJoin&)        override;
    virtual void visit(NoFreeVariableMinus&)       override;
    virtual void visit(ObjectEnum&)                override;
    virtual void visit(OrderBy&)                   override;
    virtual void visit(SingleResultBindingIter&)   override;
    virtual void visit(Slice&)                     override;
    virtual void visit(SparqlService&)             override;
    virtual void visit(SubSelect&)                 override;
    virtual void visit(Union&)                     override;
    virtual void visit(Values&)                    override;

    virtual void visit(HashJoin::BGP::Hybrid::Join1Var&)       override;
    virtual void visit(HashJoin::BGP::Hybrid::Join<2>&)        override;
    virtual void visit(HashJoin::BGP::Hybrid::Join<3>&)        override;
    virtual void visit(HashJoin::BGP::Hybrid::Join<4>&)        override;
    virtual void visit(HashJoin::BGP::InMemory::Join1Var&)     override;
    virtual void visit(HashJoin::BGP::InMemory::Join<2>&)      override;
    virtual void visit(HashJoin::BGP::InMemory::Join<3>&)      override;
    virtual void visit(HashJoin::BGP::InMemory::Join<4>&)      override;
    virtual void visit(HashJoin::Generic::Hybrid::AntiJoin&)   override;
    virtual void visit(HashJoin::Generic::Hybrid::Join&)       override;
    virtual void visit(HashJoin::Generic::Hybrid::LeftJoin&)   override;
    virtual void visit(HashJoin::Generic::Hybrid::SemiJoin&)   override;
    virtual void visit(HashJoin::Generic::InMemory::AntiJoin&) override;
    virtual void visit(HashJoin::Generic::InMemory::Join&)     override;
    virtual void visit(HashJoin::Generic::InMemory::LeftJoin&) override;
    virtual void visit(HashJoin::Generic::InMemory::SemiJoin&) override;
    virtual void visit(HashJoin::MaterializeIter&)             override;

    virtual void visit(Paths::UnfixedComposite&)                   override;
    virtual void visit(Paths::AllShortest::BFSCheck&)              override;
    virtual void visit(Paths::AllShortest::BFSEnum<false>&)        override;
    virtual void visit(Paths::AllShortest::BFSEnum<true>&)         override;
    virtual void visit(Paths::AllShortestCount::BFSCheck&)         override;
    virtual void visit(Paths::AllShortestCount::BFSEnum<false>&)   override;
    virtual void visit(Paths::AllShortestCount::BFSEnum<true>&)    override;
    virtual void visit(Paths::AllShortestSimple::BFSCheck<false>&) override;
    virtual void visit(Paths::AllShortestSimple::BFSCheck<true>&)  override;
    virtual void visit(Paths::AllShortestSimple::BFSEnum<false>&)  override;
    virtual void visit(Paths::AllShortestSimple::BFSEnum<true>&)   override;
    virtual void visit(Paths::AllShortestTrails::BFSCheck&)        override;
    virtual void visit(Paths::AllShortestTrails::BFSEnum&)         override;
    virtual void visit(Paths::AllSimple::BFSCheck<false>&)         override;
    virtual void visit(Paths::AllSimple::BFSCheck<true>&)          override;
    virtual void visit(Paths::AllSimple::BFSEnum<false>&)          override;
    virtual void visit(Paths::AllSimple::BFSEnum<true>&)           override;
    virtual void visit(Paths::AllSimple::DFSCheck<false>&)         override;
    virtual void visit(Paths::AllSimple::DFSCheck<true>&)          override;
    virtual void visit(Paths::AllSimple::DFSEnum<false>&)          override;
    virtual void visit(Paths::AllSimple::DFSEnum<true>&)           override;
    virtual void visit(Paths::AllTrails::BFSCheck&)                override;
    virtual void visit(Paths::AllTrails::BFSEnum&)                 override;
    virtual void visit(Paths::AllTrails::DFSCheck&)                override;
    virtual void visit(Paths::AllTrails::DFSEnum&)                 override;
    virtual void visit(Paths::Any::DFSCheck&)                      override;
    virtual void visit(Paths::Any::DFSEnum<false>&)                override;
    virtual void visit(Paths::Any::DFSEnum<true>&)                 override;
    virtual void visit(Paths::Any::BFS_RDPQCheck&)         override;
    virtual void visit(Paths::Any::BFS_RDPQEnum&)          override;
    virtual void visit(Paths::Any::DijkstraCheck&)         override;
    virtual void visit(Paths::Any::DijkstraEnum&)          override;
    virtual void visit(Paths::Any::BFSCheck&)              override;
    virtual void visit(Paths::Any::BFSEnum<false>&)        override;
    virtual void visit(Paths::Any::BFSEnum<true>&)         override;
    virtual void visit(Paths::Any::BFSMultipleStarts<false>&)          override;
    virtual void visit(Paths::Any::BFSMultipleStarts<true>&)           override;
    virtual void visit(Paths::Any::BFSMultipleStartsNaive<false>&)          override;
    virtual void visit(Paths::Any::BFSMultipleStartsNaive<true>&)           override;
    virtual void visit(Paths::AnySimple::BFSCheck<false>&)         override;
    virtual void visit(Paths::AnySimple::BFSCheck<true>&)          override;
    virtual void visit(Paths::AnySimple::BFSEnum<false>&)          override;
    virtual void visit(Paths::AnySimple::BFSEnum<true>&)           override;
    virtual void visit(Paths::AnySimple::DFSCheck<false>&)         override;
    virtual void visit(Paths::AnySimple::DFSCheck<true>&)          override;
    virtual void visit(Paths::AnySimple::DFSEnum<false>&)          override;
    virtual void visit(Paths::AnySimple::DFSEnum<true>&)           override;
    virtual void visit(Paths::AnyTrails::BFSCheck&)                override;
    virtual void visit(Paths::AnyTrails::BFSEnum&)                 override;
    virtual void visit(Paths::AnyTrails::DFSCheck&)                override;
    virtual void visit(Paths::AnyTrails::DFSEnum&)                 override;

    virtual void visit(LSH::ForestIndexTopK&)         override;
    virtual void visit(LSH::ForestIndexTopAll&)       override;
    virtual void visit(LSH::ProjectTensorSimilarity&) override;
    virtual void visit(LSH::BruteSimilaritySearch&)   override;
};
