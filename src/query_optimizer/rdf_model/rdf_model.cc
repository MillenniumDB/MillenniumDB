#include "rdf_model.h"

#include <iostream>
#include <new>
#include <set>

#include "base/exceptions.h"
#include "base/graph_object/anonymous_node.h"
#include "base/graph_object/edge.h"
#include "base/graph_object/path.h"
#include "base/path_printer.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "execution/graph_object/graph_object_manager.h"
#include "query_optimizer/rdf_model/binding_iter_visitor.h"
#include "query_optimizer/rdf_model/sparql_element_to_object_id.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/string_manager.h"

using namespace std;

// memory for the object
static typename std::aligned_storage<sizeof(RdfModel), alignof(RdfModel)>::type rdf_model_buf;
// global object
RdfModel& rdf_model = reinterpret_cast<RdfModel&>(rdf_model_buf);


RdfModel::Destroyer RdfModel::init(const std::string& db_folder,
                                   uint_fast32_t      shared_buffer_pool_size,
                                   uint_fast32_t      private_buffer_pool_size,
                                   uint_fast32_t      max_threads) {
    new (&rdf_model) RdfModel(db_folder, shared_buffer_pool_size, private_buffer_pool_size, max_threads);
    return RdfModel::Destroyer();
}


RdfModel::Destroyer::~Destroyer() {
    rdf_model.~RdfModel();
}


RdfModel::RdfModel(const std::string& db_folder,
                   uint_fast32_t      shared_buffer_pool_size,
                   uint_fast32_t      private_buffer_pool_size,
                   uint_fast32_t      max_threads) {
    FileManager::init(db_folder);
    BufferManager::init(shared_buffer_pool_size, private_buffer_pool_size, max_threads);
    PathManager::init(max_threads);
    StringManager::init();

    new (&catalog())       RdfCatalog("catalog.dat"); // placement new

    Path::path_printer = &path_manager;

    GraphObject::graph_object_print    = GraphObjectManager::print_rdf;
    GraphObject::graph_object_eq       = GraphObjectManager::equal;
    GraphObject::graph_object_cmp      = GraphObjectManager::compare_rdf;
    GraphObject::graph_object_sum      = GraphObjectManager::sum;
    GraphObject::graph_object_minus    = GraphObjectManager::minus;
    GraphObject::graph_object_multiply = GraphObjectManager::multiply;
    GraphObject::graph_object_divide   = GraphObjectManager::divide;
    GraphObject::graph_object_modulo   = GraphObjectManager::modulo;

    spo = make_unique<BPlusTree<3>>("spo");
    pos = make_unique<BPlusTree<3>>("pos");
    osp = make_unique<BPlusTree<3>>("osp");
    pso = make_unique<BPlusTree<3>>("pso");

    equal_spo = make_unique<BPlusTree<1>>("equal_spo");
    equal_sp  = make_unique<BPlusTree<2>>("equal_sp");
    equal_so  = make_unique<BPlusTree<2>>("equal_so");
    equal_po  = make_unique<BPlusTree<2>>("equal_po");

    equal_sp_inverted = make_unique<BPlusTree<2>>("equal_sp_inverted");
    equal_so_inverted = make_unique<BPlusTree<2>>("equal_so_inverted");
    equal_po_inverted = make_unique<BPlusTree<2>>("equal_po_inverted");
}


RdfModel::~RdfModel() {
    // Must destroy everything before buffer and file manager
    catalog().~RdfCatalog();

    spo.reset();
    pos.reset();
    osp.reset();
    pso.reset();

    equal_spo.reset();
    equal_sp.reset();
    equal_so.reset();
    equal_po.reset();
    equal_sp_inverted.reset();
    equal_so_inverted.reset();
    equal_po_inverted.reset();

    string_manager.~StringManager();
    path_manager.~PathManager();
    buffer_manager.~BufferManager();
    file_manager.~FileManager();
}


std::unique_ptr<BindingIter> RdfModel::exec(Op& op, ThreadInfo* thread_info) const {
    auto vars = op.get_vars();
    auto query_optimizer = SPARQL::BindingIterVisitor(std::move(vars), thread_info);
    op.accept_visitor(query_optimizer);
    return move(query_optimizer.tmp);
}


GraphObject RdfModel::get_graph_object(ObjectId object_id) const {
    if ( object_id.is_not_found() ) {
        return GraphObjectFactory::make_not_found();
    }
    if ( object_id.is_null() ) {
        return GraphObjectFactory::make_null();
    }
    auto mask        = object_id.id & ObjectId::TYPE_MASK;
    auto unmasked_id = object_id.id & ObjectId::VALUE_MASK;
    switch (mask) {
        case ObjectId::MASK_STRING_EXTERN : {
            return GraphObjectFactory::make_string_external(unmasked_id);
        }

        case ObjectId::MASK_STRING_INLINED : {
            char c[8];
            int shift_size = 6*8;
            for (int i = 0; i < ObjectId::MAX_INLINED_BYTES; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i] = byte;
                shift_size -= 8;
            }
            c[7] = '\0';
            return GraphObjectFactory::make_string_inlined(c);
        }

        case ObjectId::MASK_POSITIVE_INT : {
            static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
            int64_t i = object_id.id & 0x00FF'FFFF'FFFF'FFFFUL;
            return GraphObjectFactory::make_int(i);
        }

        case ObjectId::MASK_NEGATIVE_INT : {
            static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
            int64_t i = (~object_id.id) & 0x00FF'FFFF'FFFF'FFFFUL;
            return GraphObjectFactory::make_int(i*-1);
        }

        case ObjectId::MASK_FLOAT : {
            static_assert(sizeof(float) == 4, "float must be 4 bytes");
            float f;
            uint8_t* dest = (uint8_t*)&f;
            dest[0] =  object_id.id        & 0xFF;
            dest[1] = (object_id.id >> 8)  & 0xFF;
            dest[2] = (object_id.id >> 16) & 0xFF;
            dest[3] = (object_id.id >> 24) & 0xFF;
            return GraphObjectFactory::make_float(f);
        }

        case ObjectId::MASK_BOOL : {
            bool b;
            uint8_t* dest = (uint8_t*)&b;
            *dest = object_id.id & 0xFF;
            return GraphObjectFactory::make_bool(b);
        }

        case ObjectId::MASK_NAMED_NODE_EXTERN : {
            return GraphObjectFactory::make_named_node_external(unmasked_id);
        }

        case ObjectId::MASK_NAMED_NODE_INLINED : {
            char c[8];
            int shift_size = 6*8;
            for (int i = 0; i < ObjectId::MAX_INLINED_BYTES; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i] = byte;
                shift_size -= 8;
            }
            c[7] = '\0';
            return GraphObjectFactory::make_named_node_inlined(c);
        }

        case ObjectId::MASK_ANON : {
            return GraphObjectFactory::make_anonymous(unmasked_id);
        }

        case ObjectId::MASK_PATH : {
            return GraphObjectFactory::make_path(unmasked_id);
        }

        case ObjectId::MASK_EDGE : {
            return GraphObjectFactory::make_edge(unmasked_id);
        }

        case ObjectId::MASK_IRI_EXTERN: {
            return GraphObjectFactory::make_iri_external(unmasked_id);
        }

        case ObjectId::MASK_IRI_INLINED: {
            char c[7];
            int  suffix_shift_size = 5 * 8;
            for (int i = 0; i < 6; i++) {
                uint8_t byte = (object_id.id >> suffix_shift_size) & 0xFF;
                c[i]         = byte;
                suffix_shift_size -= 8;
            }
            c[6] = '\0';
            int     prefix_shift_size = 6 * 8;
            uint8_t prefix_id         = (object_id.id & 0x00FF'0000'0000'0000UL) >> prefix_shift_size;
            return GraphObjectFactory::make_iri_inlined(c, prefix_id);
        }

        case ObjectId::MASK_STRING_DATATYPE_INLINED: {
            char c[6];
            int  shift_size = 4 * 8;
            for (int i = 0; i < 5; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i]         = byte;
                shift_size -= 8;
            }
            c[5] = '\0';
            int prefix_shift_size = 5 * 8;
            uint16_t datatype_id = (object_id.id & 0x00FF'FF00'0000'0000UL) >> prefix_shift_size;
            return GraphObjectFactory::make_literal_datatype_inlined(c, datatype_id);
        }

        case ObjectId::MASK_STRING_DATATYPE_EXTERN: {
            return GraphObjectFactory::make_literal_datatype_external(unmasked_id);
        }

        case ObjectId::MASK_STRING_LANG_INLINED: {
            char c[6];
            int  shift_size = 4 * 8;
            for (int i = 0; i < 5; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i]         = byte;
                shift_size -= 8;
            }
            c[5] = '\0';
            int prefix_shift_size = 5 * 8;
            uint16_t language_id = (object_id.id & 0x00FF'FF00'0000'0000UL) >> prefix_shift_size;
            return GraphObjectFactory::make_literal_language_inlined(c, language_id);
        }

        case ObjectId::MASK_STRING_LANG_EXTERN: {
            return GraphObjectFactory::make_literal_language_external(unmasked_id);
        }

        case ObjectId::MASK_DATETIME: {
            return GraphObjectFactory::make_datetime(unmasked_id);
        }

        case ObjectId::MASK_DECIMAL_EXTERN: {
            return GraphObjectFactory::make_decimal_external(unmasked_id);
        }

        case ObjectId::MASK_DECIMAL_INLINED: {
            return GraphObjectFactory::make_decimal_inlined(unmasked_id);
        }

        default: {
            throw LogicException("Unhandled Object Type.");
        }
    }
}


ObjectId RdfModel::get_object_id(const SparqlElement& graph_object) const {
    SparqlElementToObjectId visitor(false);
    return visitor(graph_object);
}
