#pragma once

#include <type_traits>

#include "execution/graph_model.h"
#include "query_optimizer/rdf_model/rdf_catalog.h"

template <std::size_t N> class BPlusTree;

class SparqlElement;

class RdfModel /*: public GraphModel*/ {
    class Destroyer {
        friend class RdfModel;

    private:
        Destroyer() = default;

    public:
        ~Destroyer();
    };

public:
    std::unique_ptr<BPlusTree<3>> spo; // (subject,    predicate, object)
    std::unique_ptr<BPlusTree<3>> pos; // (predicate, object,     subject)
    std::unique_ptr<BPlusTree<3>> osp; // (object,    subject,    predicate)
    std::unique_ptr<BPlusTree<3>> pso; // (predicate, subject,    object)

    // Special cases
    std::unique_ptr<BPlusTree<1>> equal_spo; // (subject=predicate=object)
    std::unique_ptr<BPlusTree<2>> equal_sp;  // (subject=predicate, object)
    std::unique_ptr<BPlusTree<2>> equal_so;  // (subject=object,    predicate)
    std::unique_ptr<BPlusTree<2>> equal_po;  // (predicate=object,  subject)

    std::unique_ptr<BPlusTree<2>> equal_sp_inverted;  // (object,    subject=predicate)
    std::unique_ptr<BPlusTree<2>> equal_so_inverted;  // (predicate, subject=object)
    std::unique_ptr<BPlusTree<2>> equal_po_inverted;  // (subject,   predicate=object)


    // necessary to be called before first usage
    static RdfModel::Destroyer init(const std::string& db_folder,
                                    uint_fast32_t      shared_buffer_pool_size,
                                    uint_fast32_t      private_buffer_pool_size,
                                    uint_fast32_t      max_threads);

    std::unique_ptr<BindingIter> exec(Op&, ThreadInfo*) const /*override*/;

    // void exec_inserts(const SPARQL::OpInsert&);

    ObjectId get_object_id(const SparqlElement&) const /*override*/;

    // ObjectId get_or_create_object_id(const SparqlElement&);

    GraphObject get_graph_object(ObjectId) const /*override*/;

    // SparqlElement->ObjectId->GraphObject

    inline RdfCatalog& catalog() const noexcept {
        return const_cast<RdfCatalog&>(reinterpret_cast<const RdfCatalog&>(catalog_buf));
    }

private:
    typename std::aligned_storage<sizeof(RdfCatalog), alignof(RdfCatalog)>::type catalog_buf;

    RdfModel(const std::string& db_folder,
             uint_fast32_t      shared_buffer_pool_size,
             uint_fast32_t      private_buffer_pool_size,
             uint_fast32_t      max_threads);

    ~RdfModel();
};

extern RdfModel& rdf_model; // global object
