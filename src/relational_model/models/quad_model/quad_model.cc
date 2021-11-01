#include "quad_model.h"

#include <iostream>
#include <new>

#include "base/graph/anonymous_node.h"
#include "base/graph/edge.h"
#include "base/graph/path.h"
#include "base/graph/path_printer.h"
#include "relational_model/execution/binding_id_iter/property_paths/path_manager.h"
#include "relational_model/models/quad_model/graph_object_visitor.h"
#include "relational_model/models/quad_model/query_optimizer/binding_iter_visitor.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"

using namespace std;

PathPrinter* Path::path_printer = nullptr;


QuadModel::QuadModel(const std::string& db_folder,
                     uint_fast32_t shared_buffer_pool_size,
                     uint_fast32_t private_buffer_pool_size,
                     uint_fast32_t max_threads)
{
    FileManager::init(db_folder);
    BufferManager::init(shared_buffer_pool_size, private_buffer_pool_size, max_threads);
    PathManager::init(*this, max_threads);

    new (&catalog())       QuadCatalog("catalog.dat");                    // placement new
    new (&object_file())   ObjectFile("object_file.dat");                 // placement new
    new (&strings_hash())  ObjectFileHash(object_file(), "str_hash.dat"); // placement new

    Path::path_printer = &path_manager;

    nodes = make_unique<BPlusTree<1>>("nodes");
    edge_table = make_unique<RandomAccessTable<3>>("edges.table");

    // Create BPT
    label_node = make_unique<BPlusTree<2>>("label_node");
    node_label = make_unique<BPlusTree<2>>("node_label");

    object_key_value = make_unique<BPlusTree<3>>("object_key_value");
    key_value_object = make_unique<BPlusTree<3>>("key_value_object");

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


std::unique_ptr<BindingIter> QuadModel::exec(OpSelect& op_select, ThreadInfo* thread_info) const {
    auto vars = op_select.get_vars();
    auto query_optimizer = BindingIterVisitor(*this, std::move(vars), thread_info);
    return query_optimizer.exec(op_select);
}


GraphObject QuadModel::get_graph_object(ObjectId object_id) const {
    if ( object_id.is_not_found() ) {
        return GraphObject::make_not_found();
    }
    if ( object_id.is_null() ) {
        return GraphObject::make_null();
    }
    auto mask        = object_id.id & TYPE_MASK;
    auto unmasked_id = object_id.id & VALUE_MASK;
    switch (mask) {
        case VALUE_EXTERNAL_STR_MASK : {
            const char* str = object_file().read(unmasked_id);
            return GraphObject::make_string_external(str);
        }

        case VALUE_INLINE_STR_MASK : {
            char c[8];
            int shift_size = 6*8;
            for (int i = 0; i < MAX_INLINED_BYTES; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i] = byte;
                shift_size -= 8;
            }
            c[7] = '\0';
            return GraphObject::make_string_inlined(c);
        }

        case VALUE_POSITIVE_INT_MASK : {
            static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
            int64_t i = object_id.id & 0x00FF'FFFF'FFFF'FFFFUL;
            return GraphObject::make_int(i);
        }

        case VALUE_NEGATIVE_INT_MASK : {
            static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
            int64_t i = (~object_id.id) & 0x00FF'FFFF'FFFF'FFFFUL;
            return GraphObject::make_int(i*-1);
        }

        case VALUE_FLOAT_MASK : {
            static_assert(sizeof(float) == 4, "float must be 4 bytes");
            float f;
            uint8_t* dest = (uint8_t*)&f;
            dest[0] =  object_id.id        & 0xFF;
            dest[1] = (object_id.id >> 8)  & 0xFF;
            dest[2] = (object_id.id >> 16) & 0xFF;
            dest[3] = (object_id.id >> 24) & 0xFF;
            return GraphObject::make_float(f);
        }

        case VALUE_BOOL_MASK : {
            bool b;
            uint8_t* dest = (uint8_t*)&b;
            *dest = object_id.id & 0xFF;
            return GraphObject::make_bool(b);
        }

        case IDENTIFIABLE_EXTERNAL_MASK : {
            const char* str = object_file().read(unmasked_id);
            return GraphObject::make_identifiable_external(str);
        }

        case IDENTIFIABLE_INLINED_MASK : {
            char c[8];
            int shift_size = 6*8;
            for (int i = 0; i < MAX_INLINED_BYTES; i++) {
                uint8_t byte = (object_id.id >> shift_size) & 0xFF;
                c[i] = byte;
                shift_size -= 8;
            }
            c[7] = '\0';
            return GraphObject::make_identifiable_inlined(c);
        }

        case ANONYMOUS_NODE_MASK : {
            return GraphObject::make_anonymous(unmasked_id);
        }

        case CONNECTION_MASK : {
            return GraphObject::make_edge(unmasked_id);
        }

        case VALUE_PATH_MASK : {
            return GraphObject::make_path(unmasked_id);
        }

        default : {
            throw std::logic_error("Unhandled Object Type.");
        }
    }
}


GraphObject QuadModel::get_property_value(GraphObject& obj, const ObjectId key) const {
    auto obj_id = get_object_id(obj);

    bool interruption_requested = false;
    auto it = object_key_value->get_range(&interruption_requested,
                                          RecordFactory::get(obj_id.id, key.id, 0),
                                          RecordFactory::get(obj_id.id, key.id, UINT64_MAX));

    auto res = it->next();
    if (res != nullptr) {
        auto value_id = ObjectId(res->ids[2]);
        return get_graph_object(value_id);
    } else {
        return GraphObject::make_null();
    }
}


ObjectId QuadModel::get_object_id(const GraphObject& graph_object) const {
    return std::visit(GraphObjectVisitor(*this, false), graph_object.value);
}


uint64_t QuadModel::get_or_create_object_id(const GraphObject& graph_object) {
    return std::visit(GraphObjectVisitor(*this, true), graph_object.value).id;
}


ObjectId QuadModel::get_value_id(const common::ast::Value& value) const {
    GraphObject graph_object;
    if (value.type() == typeid(string)) {
        auto str = boost::get<string>(value);
        graph_object = GraphObject::make_string(str.c_str());
    }
    else if (value.type() == typeid(int64_t)) {
        graph_object = GraphObject::make_int( boost::get<int64_t>(value) );
    }
    else if (value.type() == typeid(float)) {
        graph_object = GraphObject::make_float( boost::get<float>(value) );
    }
    else if (value.type() == typeid(bool)) {
        graph_object = GraphObject::make_bool( boost::get<bool>(value) );
    }
    else {
        throw logic_error("Unknown value type.");
    }
    return get_object_id(graph_object);
}