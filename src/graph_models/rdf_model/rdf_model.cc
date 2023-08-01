#include "rdf_model.h"

#include <new>
#include <set>

#include "graph_models/inliner.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "graph_models/rdf_model/datatypes/decimal.h"
#include "query/executor/binding_iter/paths/path_manager.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/string_manager.h"
#include "storage/tmp_manager.h"

using namespace std;

// memory for the object
static typename std::aligned_storage<sizeof(RdfModel), alignof(RdfModel)>::type rdf_model_buf;
// global object
RdfModel& rdf_model = reinterpret_cast<RdfModel&>(rdf_model_buf);


RdfModel::Destroyer RdfModel::init(const std::string& db_folder,
                                   uint_fast32_t      str_max_initial_populate_size,
                                   uint_fast32_t      shared_buffer_pool_size,
                                   uint_fast32_t      private_buffer_pool_size,
                                   uint_fast32_t      max_threads) {
    new (&rdf_model) RdfModel(db_folder, str_max_initial_populate_size, shared_buffer_pool_size, private_buffer_pool_size, max_threads);
    return RdfModel::Destroyer();
}


RdfModel::Destroyer::~Destroyer() {
    rdf_model.~RdfModel();
}

std::ostream& rdf_debug_print(std::ostream& os, ObjectId object_id) {
    const auto mask        = object_id.id & ObjectId::TYPE_MASK;
    const auto unmasked_id = object_id.id & ObjectId::VALUE_MASK;
    switch (mask) {
    case ObjectId::MASK_NULL: {
        return os << "null";
    }
    case ObjectId::MASK_ANON_INLINED: {
        return os << "_:b" << unmasked_id;
    }
    case ObjectId::MASK_ANON_TMP: {
        return os << "_:c" << unmasked_id;
    }
    case ObjectId::MASK_IRI_INLINED: {
        uint8_t prefix_id = (object_id.id & ObjectId::MASK_IRI_PREFIX) >> (8*ObjectId::IRI_INLINE_BYTES);
        os << rdf_model.catalog().prefixes[prefix_id];
        Inliner::print_string_inlined<6>(os, unmasked_id);
        break;
    }
    case ObjectId::MASK_IRI_EXTERN: {
        uint64_t iri_id = unmasked_id & ObjectId::MASK_IRI_CONTENT;
        uint8_t prefix_id = (unmasked_id & ObjectId::MASK_IRI_PREFIX) >> 48;

        os << rdf_model.catalog().prefixes[prefix_id];
        string_manager.print(os, iri_id);
        break;
    }
    case ObjectId::MASK_IRI_TMP: {
        uint8_t prefix_id = (unmasked_id & ObjectId::MASK_IRI_PREFIX) >> 48;
        uint64_t iri_id = unmasked_id & ObjectId::MASK_IRI_CONTENT;

        os << rdf_model.catalog().prefixes[prefix_id];
        tmp_manager.print_str(os, iri_id); // gets string from id
        break;
    }
    case ObjectId::MASK_STRING_XSD_INLINED:
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        Inliner::print_string_inlined<7>(os, unmasked_id);
        break;
    }
    case ObjectId::MASK_STRING_XSD_EXTERN:
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        string_manager.print(os, unmasked_id);
        break;
    }
    case ObjectId::MASK_STRING_XSD_TMP:
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        tmp_manager.print_str(os, unmasked_id);
        break;
    }
    case ObjectId::MASK_STRING_LANG_INLINED: {
        int prefix_shift_size = 8 * ObjectId::STR_LANG_INLINE_BYTES;
        uint16_t language_id = (object_id.id & ObjectId::MASK_LITERAL_TAG) >> prefix_shift_size;

        Inliner::print_string_inlined<5>(os, unmasked_id);
        os << '@';
        if ((language_id & ObjectId::MASK_TAG_MANAGER) == 0) {
            os << rdf_model.catalog().languages[language_id];
        } else {
            tmp_manager.print_lan(os, (language_id & (~ObjectId::MASK_TAG_MANAGER)));
        }
        break;
    }
    case ObjectId::MASK_STRING_LANG_EXTERN: {
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        uint16_t language_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;

        string_manager.print(os, str_id);
        os << '@';
        if ((language_id & ObjectId::MASK_TAG_MANAGER) == 0) {
            os << rdf_model.catalog().languages[language_id];
        } else {
            tmp_manager.print_lan(os, (language_id & (~ObjectId::MASK_TAG_MANAGER)));
        }
        break;
    }
    case ObjectId::MASK_STRING_LANG_TMP: {
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        uint64_t language_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;

        tmp_manager.print_str(os, str_id);
        os << '@';
        if ((language_id & ObjectId::MASK_TAG_MANAGER) == 0) {
            os << rdf_model.catalog().languages[language_id];
        } else {
            tmp_manager.print_lan(os, (language_id & (~ObjectId::MASK_TAG_MANAGER)));
        }
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_INLINED: {
        int prefix_shift_size = 8 * ObjectId::STR_DT_INLINE_BYTES;
        uint16_t datatype_id = (object_id.id & ObjectId::MASK_LITERAL_TAG) >> prefix_shift_size;

        Inliner::print_string_inlined<5>(os, unmasked_id);
        os << "^^";
        if ((datatype_id & ObjectId::MASK_TAG_MANAGER) == 0) {
            os << rdf_model.catalog().datatypes[datatype_id];
        } else {
            tmp_manager.print_dtt(os, (datatype_id & (~ObjectId::MASK_TAG_MANAGER)));
        }
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_EXTERN: {
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        uint16_t datatype_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;

        string_manager.print(os, str_id);
        os << "^^";
        if ((datatype_id & ObjectId::MASK_TAG_MANAGER) == 0) {
            os << rdf_model.catalog().datatypes[datatype_id];
        } else {
            tmp_manager.print_dtt(os, (datatype_id & (~ObjectId::MASK_TAG_MANAGER)));
        }

        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_TMP: {
        uint64_t str_id = unmasked_id & ObjectId::MASK_LITERAL;
        uint64_t datatype_id = (unmasked_id & ObjectId::MASK_LITERAL_TAG) >> 40;

        tmp_manager.print_str(os, str_id);
        os << "^^";
        if ((datatype_id & ObjectId::MASK_TAG_MANAGER) == 0) {
            os << rdf_model.catalog().datatypes[datatype_id];
        } else {
            tmp_manager.print_dtt(os, (datatype_id & (~ObjectId::MASK_TAG_MANAGER)));
        }
        break;
    }
    case ObjectId::MASK_NEGATIVE_INT: {
        int64_t i = (~object_id.id) & 0x00FF'FFFF'FFFF'FFFFUL;
        return os << (i*-1);
    }
    case ObjectId::MASK_POSITIVE_INT: {
        int64_t i = unmasked_id;
        return os << i;
    }
    case ObjectId::MASK_DECIMAL_INLINED: {
        DecimalInlined decimal_inlined(unmasked_id);
        os << decimal_inlined.get_value_string();
        break;
    }
    case ObjectId::MASK_DECIMAL_EXTERN: {
        std::stringstream ss;
        string_manager.print(ss, unmasked_id);
        Decimal dec;
        dec.from_external(ss.str());
        os << dec;
        break;
    }
    case ObjectId::MASK_DECIMAL_TMP: {
        std::ostringstream ss;
        tmp_manager.print_str(ss, unmasked_id);
        Decimal dec;
        dec.from_external(ss.str());
        os << dec;
        break;
    }
    case ObjectId::MASK_FLOAT: {
        float f;
        uint8_t* dest = reinterpret_cast<uint8_t*>(&f);
        dest[0] =  object_id.id        & 0xFF;
        dest[1] = (object_id.id >> 8)  & 0xFF;
        dest[2] = (object_id.id >> 16) & 0xFF;
        dest[3] = (object_id.id >> 24) & 0xFF;

        os << f;
        break;
    }
    case ObjectId::MASK_DOUBLE_EXTERN: {
        std::stringstream ss;
        string_manager.print(ss, unmasked_id);
        double dbl;
        auto dst = reinterpret_cast<char*>(&dbl);
        ss.read(dst, 8);
        return os << dbl;
    }
    case ObjectId::MASK_DOUBLE_TMP: {
        std::stringstream ss;
        tmp_manager.print_str(ss, unmasked_id);
        double dbl;
        auto dst = reinterpret_cast<char*>(&dbl);
        ss.read(dst, 8);
        return os << dbl;
    }
    break;
    case ObjectId::MASK_DT_DATE:
    case ObjectId::MASK_DT_DATETIME:
    case ObjectId::MASK_DT_TIME:
    case ObjectId::MASK_DT_DATETIMESTAMP: {
        DateTime datetime(object_id);
        os << datetime.get_value_string();
        break;
    }
    case ObjectId::MASK_BOOL: {
        os << (object_id.get_value() == 0 ? "false" : "true");
        break;
    }
    // case ObjectId::MASK_PATH: {
    //     break;
    // }

    default:
        throw std::logic_error("Unmanaged mask in RdfModel::debug_print: "
            + std::to_string(mask));
    }
    return os;
}

RdfModel::RdfModel(const std::string& db_folder,
                   uint_fast32_t      str_max_initial_populate_size,
                   uint_fast32_t      shared_buffer_pool_size,
                   uint_fast32_t      private_buffer_pool_size,
                   uint_fast32_t      max_threads)
{
    FileManager::init(db_folder);
    BufferManager::init(shared_buffer_pool_size, private_buffer_pool_size, max_threads);
    PathManager::init(max_threads);
    StringManager::init(str_max_initial_populate_size);
    TmpManager::init(max_threads);

    QueryContext::_debug_print = rdf_debug_print;

    new (&catalog()) RdfCatalog("catalog.dat"); // placement new

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

    tmp_manager.~TmpManager();
    string_manager.~StringManager();
    path_manager.~PathManager();
    buffer_manager.~BufferManager();
    file_manager.~FileManager();
}
