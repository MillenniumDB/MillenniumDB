#include "quad_model.h"

#include <iostream>
#include <new>

#include "base/exceptions.h"
#include "base/graph_object/anonymous_node.h"
#include "base/graph_object/edge.h"
#include "base/graph_object/path.h"
#include "base/graph_object/named_node_external.h"
#include "base/path_printer.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "query_optimizer/quad_model/graph_object_visitor.h"
#include "query_optimizer/quad_model/binding_iter_visitor.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"

using namespace std;

PathPrinter* Path::path_printer = nullptr;
StringManager* StringManager::instance = nullptr;

// memory for the object
static typename std::aligned_storage<sizeof(QuadModel), alignof(QuadModel)>::type quad_model_buf;
// global object
QuadModel& quad_model = reinterpret_cast<QuadModel&>(quad_model_buf);


QuadModel::Destroyer QuadModel::init(const std::string& db_folder,
                                     uint_fast32_t      shared_buffer_pool_size,
                                     uint_fast32_t      private_buffer_pool_size,
                                     uint_fast32_t      max_threads)
{
    new (&quad_model) QuadModel(db_folder,
                                shared_buffer_pool_size,
                                private_buffer_pool_size,
                                max_threads);
    return QuadModel::Destroyer();
}


QuadModel::Destroyer::~Destroyer() {
    quad_model.~QuadModel();
}


QuadModel::QuadModel(const std::string& db_folder,
                     uint_fast32_t shared_buffer_pool_size,
                     uint_fast32_t private_buffer_pool_size,
                     uint_fast32_t max_threads)
{
    FileManager::init(db_folder);
    BufferManager::init(shared_buffer_pool_size, private_buffer_pool_size, max_threads);
    PathManager::init(max_threads);

    new (&catalog())       QuadCatalog("catalog.dat");                // placement new
    new (&object_file())   ObjectFile("object_file.dat");             // placement new
    new (&strings_hash())  ObjectFileHash(object_file(), "str_hash"); // placement new

    Path::path_printer = &path_manager;
    StringManager::instance = &object_file();

    nodes = make_unique<BPlusTree<1>>("nodes");
    edge_table = make_unique<RandomAccessTable<3>>("edges.table");

    // Create BPT
    label_node = make_unique<BPlusTree<2>>("label_node");
    node_label = make_unique<BPlusTree<2>>("node_label");

    object_key_value = make_unique<BPlusTree<3>>("object_key_value");
    key_value_object = make_unique<BPlusTree<3>>("key_value_object");

    // from_edge = make_unique<BPlusTree<2>>("from_edge");
    // to_edge   = make_unique<BPlusTree<2>>("to_edge");
    // type_edge = make_unique<BPlusTree<2>>("type_edge");

    from_to_type_edge = make_unique<BPlusTree<4>>("from_to_type_edge");
    to_type_from_edge = make_unique<BPlusTree<4>>("to_type_from_edge");
    type_from_to_edge = make_unique<BPlusTree<4>>("type_from_to_edge");
    type_to_from_edge = make_unique<BPlusTree<4>>("type_to_from_edge");

    equal_from_to      = make_unique<BPlusTree<3>>("equal_from_to");
    equal_from_type    = make_unique<BPlusTree<3>>("equal_from_type");
    equal_to_type      = make_unique<BPlusTree<3>>("equal_to_type");
    equal_from_to_type = make_unique<BPlusTree<2>>("equal_from_to_type");

    equal_from_to_inverted   = make_unique<BPlusTree<3>>("equal_from_to_inverted");
    equal_from_type_inverted = make_unique<BPlusTree<3>>("equal_from_type_inverted");
    equal_to_type_inverted   = make_unique<BPlusTree<3>>("equal_to_type_inverted");
}


QuadModel::~QuadModel() {
    // Must destroy everything before buffer and file manager
    strings_hash().~ObjectFileHash();
    object_file().~ObjectFile();
    catalog().~QuadCatalog();

    nodes.reset();
    edge_table.reset();

    label_node.reset();
    node_label.reset();

    // from_edge.reset();
    // to_edge.reset();
    // type_edge.reset();

    object_key_value.reset();
    key_value_object.reset();

    from_to_type_edge.reset();
    to_type_from_edge.reset();
    type_from_to_edge.reset();
    type_to_from_edge.reset();

    equal_from_to.reset();
    equal_from_type.reset();
    equal_to_type.reset();
    equal_from_to_type.reset();

    equal_from_to_inverted.reset();
    equal_from_type_inverted.reset();
    equal_to_type_inverted.reset();

    path_manager.~PathManager();
    buffer_manager.~BufferManager();
    file_manager.~FileManager();
}


std::unique_ptr<BindingIter> QuadModel::exec(Op& op, ThreadInfo* thread_info) const {
    auto vars = op.get_vars();
    auto query_optimizer = BindingIterVisitor(std::move(vars), thread_info);
    op.accept_visitor(query_optimizer);
    return move(query_optimizer.tmp);
}


GraphObject QuadModel::get_graph_object(ObjectId object_id) const {
    if ( object_id.is_not_found() ) {
        return GraphObject::make_not_found();
    }
    if ( object_id.is_null() ) {
        return GraphObject::make_null();
    }
    auto mask        = object_id.id & ObjectId::TYPE_MASK;
    auto unmasked_id = object_id.id & ObjectId::VALUE_MASK;
    switch (mask) {
        case ObjectId::VALUE_EXTERNAL_STR_MASK : {
            return GraphObject::make_string_external(unmasked_id);
        }

        case ObjectId::VALUE_INLINE_STR_MASK : {
            char c[8];
            int shift_size = 6*8;
            for (int i = 0; i < ObjectId::MAX_INLINED_BYTES; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i] = byte;
                shift_size -= 8;
            }
            c[7] = '\0';
            return GraphObject::make_string_inlined(c);
        }

        case ObjectId::VALUE_POSITIVE_INT_MASK : {
            static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
            int64_t i = object_id.id & 0x00FF'FFFF'FFFF'FFFFUL;
            return GraphObject::make_int(i);
        }

        case ObjectId::VALUE_NEGATIVE_INT_MASK : {
            static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
            int64_t i = (~object_id.id) & 0x00FF'FFFF'FFFF'FFFFUL;
            return GraphObject::make_int(i*-1);
        }

        case ObjectId::VALUE_FLOAT_MASK : {
            static_assert(sizeof(float) == 4, "float must be 4 bytes");
            float f;
            uint8_t* dest = (uint8_t*)&f;
            dest[0] =  object_id.id        & 0xFF;
            dest[1] = (object_id.id >> 8)  & 0xFF;
            dest[2] = (object_id.id >> 16) & 0xFF;
            dest[3] = (object_id.id >> 24) & 0xFF;
            return GraphObject::make_float(f);
        }

        case ObjectId::VALUE_BOOL_MASK : {
            bool b;
            uint8_t* dest = (uint8_t*)&b;
            *dest = object_id.id & 0xFF;
            return GraphObject::make_bool(b);
        }

        case ObjectId::IDENTIFIABLE_EXTERNAL_MASK : {
            return GraphObject::make_named_node_external(unmasked_id);
        }

        case ObjectId::IDENTIFIABLE_INLINED_MASK : {
            char c[8];
            int shift_size = 6*8;
            for (int i = 0; i < ObjectId::MAX_INLINED_BYTES; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i] = byte;
                shift_size -= 8;
            }
            c[7] = '\0';
            return GraphObject::make_named_node_inlined(c);
        }

        case ObjectId::ANONYMOUS_NODE_MASK : {
            return GraphObject::make_anonymous(unmasked_id);
        }

        case ObjectId::CONNECTION_MASK : {
            return GraphObject::make_edge(unmasked_id);
        }

        case ObjectId::VALUE_PATH_MASK : {
            return GraphObject::make_path(unmasked_id);
        }

        default : {
            throw LogicException("Unhandled Object Type.");
        }
    }
}


ObjectId QuadModel::get_object_id(const GraphObject& graph_object) const {
    // return std::visit(GraphObjectVisitor(false), graph_object.value);
    GraphObjectVisitor visitor(false);
    return visitor(graph_object);
}


uint64_t QuadModel::get_or_create_object_id(const GraphObject& graph_object) {
    // return std::visit(GraphObjectVisitor(true), graph_object.value).id;
    GraphObjectVisitor visitor(true);
    return visitor(graph_object).id;
}
