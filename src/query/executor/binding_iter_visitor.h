#pragma once

#include <cstddef>


class Aggregation;
class Bind;
class CrossProduct;
class DistinctHash;
class EdgeTableLookup;
class EmptyBindingIter;
class Filter;
class IndexLeftOuterJoin;
class IndexNestedLoopJoin;
template <std::size_t> class IndexScan;
class LeapfrogJoin;
template<bool> class NestedLoopAntiJoin;
class NestedLoopJoin;
class NestedLoopLeftJoin;
class NestedLoopSemiJoin;
class NoFreeVariableMinus;
class ObjectEnum;
class OrderBy;
class ExprEvaluator;
class SingleResultBindingIter;
class Slice;
class SparqlService;
class SubSelect;
class Union;
class Values;


class CrossProduct;
class HashJoinGrace;
class HashJoinInBuffer;
class HashJoinInMemory;
class LeftCrossProduct;

namespace LSH {
class ForestIndexTopK;
class ForestIndexTopAll;
class ProjectTensorSimilarity;
class BruteSimilaritySearch;
}


namespace HashJoin {
    class MaterializeIter;
    namespace BGP {
        namespace Hybrid {
            class Join1Var;
            template<std::size_t N> class Join;
        }
        namespace InMemory {
            class Join1Var;
            template<std::size_t N> class Join;
        }
    }
    namespace Generic {
        namespace Hybrid {
            class AntiJoin;
            class Join;
            class LeftJoin;
            class SemiJoin;
        }
        namespace InMemory {
            class AntiJoin;
            class Join;
            class LeftJoin;
            class SemiJoin;
        }
    }
} // namespace HashJoin


namespace Paths {
    class UnfixedComposite;
    namespace AllShortest {
        class BFSCheck;
        template <bool> class BFSEnum;
    }
    namespace AllShortestCount {
        class BFSCheck;
        template <bool> class BFSEnum;
    }
    namespace AllShortestSimple {
        template <bool> class BFSCheck;
        template <bool> class BFSEnum;
    }
    namespace AllShortestTrails {
        class BFSCheck;
        class BFSEnum;
    }
    namespace AllSimple {
        template <bool> class BFSCheck;
        template <bool> class BFSEnum;
        template <bool> class DFSCheck;
        template <bool> class DFSEnum;
    }
    namespace AllTrails {
        class BFSCheck;
        class BFSEnum;
        class DFSCheck;
        class DFSEnum;
    }
    namespace Any {
        class BFSCheck;
        template <bool> class BFSEnum;
        template <bool> class BFSMultipleStarts;
        template <bool> class BFSMultipleStartsNaive;
        class DFSCheck;
        template <bool> class DFSEnum;
        class BFS_RDPQCheck;
        class BFS_RDPQEnum;
        class DijkstraCheck;
        class DijkstraEnum;
    }
    namespace AnySimple {
        template <bool> class BFSCheck;
        template <bool> class BFSEnum;
        template <bool> class DFSCheck;
        template <bool> class DFSEnum;
    }
    namespace AnyTrails {
        class BFSCheck;
        class BFSEnum;
        class DFSCheck;
        class DFSEnum;
    }
} // namespace Paths


class BindingIterVisitor {
public:
    virtual void visit(Aggregation&)               = 0;
    virtual void visit(Bind&)                      = 0;
    virtual void visit(CrossProduct&)              = 0;
    virtual void visit(DistinctHash&)              = 0;
    virtual void visit(EdgeTableLookup&)           = 0;
    virtual void visit(EmptyBindingIter&)          = 0;
    virtual void visit(Filter&)                    = 0;
    virtual void visit(ExprEvaluator&)             = 0;
    virtual void visit(IndexLeftOuterJoin&)        = 0;
    virtual void visit(IndexNestedLoopJoin&)       = 0;
    virtual void visit(IndexScan<1>&)              = 0;
    virtual void visit(IndexScan<2>&)              = 0;
    virtual void visit(IndexScan<3>&)              = 0;
    virtual void visit(IndexScan<4>&)              = 0;
    virtual void visit(LeapfrogJoin&)              = 0;
    virtual void visit(NestedLoopAntiJoin<false>&) = 0;
    virtual void visit(NestedLoopAntiJoin<true>&)  = 0;
    virtual void visit(NestedLoopJoin&)            = 0;
    virtual void visit(NestedLoopLeftJoin&)        = 0;
    virtual void visit(NestedLoopSemiJoin&)        = 0;
    virtual void visit(NoFreeVariableMinus&)       = 0;
    virtual void visit(ObjectEnum&)                = 0;
    virtual void visit(OrderBy&)                   = 0;
    virtual void visit(SingleResultBindingIter&)   = 0;
    virtual void visit(Slice&)                     = 0;
    virtual void visit(SparqlService&)             = 0;
    virtual void visit(SubSelect&)                 = 0;
    virtual void visit(Union&)                     = 0;
    virtual void visit(Values&)                    = 0;


    virtual void visit(HashJoin::BGP::Hybrid::Join1Var&)       = 0;
    virtual void visit(HashJoin::BGP::Hybrid::Join<2>&)        = 0;
    virtual void visit(HashJoin::BGP::Hybrid::Join<3>&)        = 0;
    virtual void visit(HashJoin::BGP::Hybrid::Join<4>&)        = 0;
    virtual void visit(HashJoin::BGP::InMemory::Join1Var&)     = 0;
    virtual void visit(HashJoin::BGP::InMemory::Join<2>&)      = 0;
    virtual void visit(HashJoin::BGP::InMemory::Join<3>&)      = 0;
    virtual void visit(HashJoin::BGP::InMemory::Join<4>&)      = 0;
    virtual void visit(HashJoin::Generic::Hybrid::AntiJoin&)   = 0;
    virtual void visit(HashJoin::Generic::Hybrid::Join&)       = 0;
    virtual void visit(HashJoin::Generic::Hybrid::LeftJoin&)   = 0;
    virtual void visit(HashJoin::Generic::Hybrid::SemiJoin&)   = 0;
    virtual void visit(HashJoin::Generic::InMemory::AntiJoin&) = 0;
    virtual void visit(HashJoin::Generic::InMemory::Join&)     = 0;
    virtual void visit(HashJoin::Generic::InMemory::LeftJoin&) = 0;
    virtual void visit(HashJoin::Generic::InMemory::SemiJoin&) = 0;
    virtual void visit(HashJoin::MaterializeIter&)             = 0;

    virtual void visit(Paths::UnfixedComposite&)                   = 0;
    virtual void visit(Paths::AllShortest::BFSCheck&)              = 0;
    virtual void visit(Paths::AllShortest::BFSEnum<false>&)        = 0;
    virtual void visit(Paths::AllShortest::BFSEnum<true>&)         = 0;
    virtual void visit(Paths::AllShortestCount::BFSCheck&)         = 0;
    virtual void visit(Paths::AllShortestCount::BFSEnum<false>&)   = 0;
    virtual void visit(Paths::AllShortestCount::BFSEnum<true>&)    = 0;
    virtual void visit(Paths::AllShortestSimple::BFSCheck<false>&) = 0;
    virtual void visit(Paths::AllShortestSimple::BFSCheck<true>&)  = 0;
    virtual void visit(Paths::AllShortestSimple::BFSEnum<false>&)  = 0;
    virtual void visit(Paths::AllShortestSimple::BFSEnum<true>&)   = 0;
    virtual void visit(Paths::AllShortestTrails::BFSCheck&)        = 0;
    virtual void visit(Paths::AllShortestTrails::BFSEnum&)         = 0;
    virtual void visit(Paths::AllSimple::BFSCheck<false>&)         = 0;
    virtual void visit(Paths::AllSimple::BFSCheck<true>&)          = 0;
    virtual void visit(Paths::AllSimple::BFSEnum<false>&)          = 0;
    virtual void visit(Paths::AllSimple::BFSEnum<true>&)           = 0;
    virtual void visit(Paths::AllSimple::DFSCheck<false>&)         = 0;
    virtual void visit(Paths::AllSimple::DFSCheck<true>&)          = 0;
    virtual void visit(Paths::AllSimple::DFSEnum<false>&)          = 0;
    virtual void visit(Paths::AllSimple::DFSEnum<true>&)           = 0;
    virtual void visit(Paths::AllTrails::BFSCheck&)                = 0;
    virtual void visit(Paths::AllTrails::BFSEnum&)                 = 0;
    virtual void visit(Paths::AllTrails::DFSCheck&)                = 0;
    virtual void visit(Paths::AllTrails::DFSEnum&)                 = 0;
    virtual void visit(Paths::Any::DFSCheck&)                      = 0;
    virtual void visit(Paths::Any::DFSEnum<false>&)                = 0;
    virtual void visit(Paths::Any::DFSEnum<true>&)                 = 0;
    virtual void visit(Paths::Any::BFS_RDPQCheck&)                 = 0;
    virtual void visit(Paths::Any::BFS_RDPQEnum&)                  = 0;
    virtual void visit(Paths::Any::DijkstraCheck&)                 = 0;
    virtual void visit(Paths::Any::DijkstraEnum&)                  = 0;
    virtual void visit(Paths::Any::BFSCheck&)                      = 0;
    virtual void visit(Paths::Any::BFSEnum<false>&)                = 0;
    virtual void visit(Paths::Any::BFSEnum<true>&)                 = 0;
    virtual void visit(Paths::Any::BFSMultipleStarts<false>&)                = 0;
    virtual void visit(Paths::Any::BFSMultipleStarts<true>&)                 = 0;
    virtual void visit(Paths::Any::BFSMultipleStartsNaive<true>&)                 = 0;
    virtual void visit(Paths::Any::BFSMultipleStartsNaive<false>&)                = 0;
    virtual void visit(Paths::AnySimple::BFSCheck<false>&)         = 0;
    virtual void visit(Paths::AnySimple::BFSCheck<true>&)          = 0;
    virtual void visit(Paths::AnySimple::BFSEnum<false>&)          = 0;
    virtual void visit(Paths::AnySimple::BFSEnum<true>&)           = 0;
    virtual void visit(Paths::AnySimple::DFSCheck<false>&)         = 0;
    virtual void visit(Paths::AnySimple::DFSCheck<true>&)          = 0;
    virtual void visit(Paths::AnySimple::DFSEnum<false>&)          = 0;
    virtual void visit(Paths::AnySimple::DFSEnum<true>&)           = 0;
    virtual void visit(Paths::AnyTrails::BFSCheck&)                = 0;
    virtual void visit(Paths::AnyTrails::BFSEnum&)                 = 0;
    virtual void visit(Paths::AnyTrails::DFSCheck&)                = 0;
    virtual void visit(Paths::AnyTrails::DFSEnum&)                 = 0;

    virtual void visit(LSH::ForestIndexTopK&) = 0;
    virtual void visit(LSH::ForestIndexTopAll&) = 0;
    virtual void visit(LSH::ProjectTensorSimilarity&) = 0;
    virtual void visit(LSH::BruteSimilaritySearch&) = 0;
};
