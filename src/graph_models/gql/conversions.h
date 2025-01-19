#pragma once

#include <cstdint>
#include <string>

#include "graph_models/common/conversions.h"
#include "graph_models/common/datatypes/decimal.h"
#include "graph_models/gql/gql_object_id.h"

namespace GQL { namespace Conversions {

    using namespace Common::Conversions;

    static constexpr uint64_t LAST_TMP_ID = ObjectId::MASK_LITERAL_TAG;

    static constexpr uint64_t TMP_SHIFT = 44;

    std::string unpack_string(ObjectId oid);

    void print_string(ObjectId oid, std::ostream&);
    void print_path_node(std::ostream& os, ObjectId node_id);
    void print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse);

    ObjectId pack_edge_label(const std::string& label);
    ObjectId pack_node_label(const std::string& label);

    ObjectId pack_node_property(const std::string& key);
    ObjectId pack_edge_property(const std::string& key);

    ObjectId pack_list(const std::vector<ObjectId>& list);
    void unpack_list(ObjectId list_id, std::vector<ObjectId>& out);

    std::ostream& debug_print(std::ostream& os, ObjectId oid);

    constexpr uint64_t LIST_FILE_ID_MASK = 0x00FF'FF00'0000'0000UL;
    constexpr uint64_t LIST_OFFSET_MASK = 0x0000'00FF'FFFF'FFFFUL;

    constexpr uint64_t DECIMAL_SIGN_MASK      = 0x0080'0000'0000'0000UL;
    constexpr uint64_t DECIMAL_NUMBER_MASK    = 0x007F'FFFF'FFFF'FFF0UL;
    constexpr uint64_t DECIMAL_SEPARATOR_MASK = 0x0000'0000'0000'000FUL;
    // constexpr uint64_t FLOAT_SIGN_MASK        = 0x0000'0000'8000'0000UL;

    // The order, int < dec < flt < inv is important
    constexpr uint8_t OPTYPE_INTEGER = 0x01;
    constexpr uint8_t OPTYPE_DECIMAL = 0x02;
    constexpr uint8_t OPTYPE_FLOAT   = 0x03;
    constexpr uint8_t OPTYPE_DOUBLE  = 0x04;
    constexpr uint8_t OPTYPE_INVALID = 0x05;

    Decimal unpack_decimal(ObjectId oid);
    Decimal unpack_decimal_inlined(ObjectId oid);

    uint8_t calculate_optype(ObjectId oid1, ObjectId oid2);
    uint8_t calculate_optype(ObjectId oid);

    ObjectId pack_string_simple(const std::string& str);
    ObjectId pack_decimal(Decimal dec);
    ObjectId pack_double(double dbl);

    int64_t  to_integer(ObjectId oid);
    Decimal  to_decimal(ObjectId oid);
    float    to_float(ObjectId oid);
    double   to_double(ObjectId oid);
    ObjectId to_boolean(ObjectId oid);

    // Returns a string with the lexical representation of the value
    std::string to_lexical_str(ObjectId oid);

    std::ostream& debug_print(std::ostream& os, ObjectId oid);

}} // namespace GQL::Conversions
