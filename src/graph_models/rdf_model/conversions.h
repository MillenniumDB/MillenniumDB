#pragma once

#include <cstdint>
#include <string>

#include "graph_models/inliner.h"
#include "graph_models/object_id.h"
#include "graph_models/rdf_model/datatypes/decimal.h"
#include "graph_models/rdf_model/datatypes/decimal_inlined.h"
#include "graph_models/rdf_model/rdf_model.h"


namespace SPARQL {
class Conversions {
public:
    static constexpr uint64_t DECIMAL_SIGN_MASK      = 0x0080'0000'0000'0000UL;
    static constexpr uint64_t DECIMAL_NUMBER_MASK    = 0x007F'FFFF'FFFF'FFF0UL;
    static constexpr uint64_t DECIMAL_SEPARATOR_MASK = 0x0000'0000'0000'000FUL;
    static constexpr uint64_t FLOAT_SIGN_MASK        = 0x0000'0000'8000'0000UL;

    static constexpr int64_t  INTEGER_MAX            = 0x00FF'FFFF'FFFF'FFFFL;

    // The order, int < dec < flt < inv is important
    static constexpr uint8_t OPTYPE_INTEGER = 0x01;
    static constexpr uint8_t OPTYPE_DECIMAL = 0x02;
    static constexpr uint8_t OPTYPE_FLOAT   = 0x03;
    static constexpr uint8_t OPTYPE_DOUBLE  = 0x04;
    static constexpr uint8_t OPTYPE_INVALID = 0x05;

    static int64_t unpack_int(ObjectId oid);
    static Decimal unpack_decimal(ObjectId oid);
    static float   unpack_float(ObjectId oid);
    static double  unpack_double(ObjectId oid);

    static std::string unpack_named_node(ObjectId oid);
    static std::string unpack_string_simple(ObjectId oid);
    static std::string unpack_string_xsd(ObjectId oid);
    static std::pair<uint16_t, std::string> unpack_string_lang(ObjectId oid);
    static std::pair<uint16_t, std::string> unpack_string_datatype(ObjectId oid);
    static std::string unpack_iri(ObjectId oid);

    static uint8_t calculate_optype(ObjectId oid1, ObjectId oid2);
    static uint8_t calculate_optype(ObjectId oid);

    static ObjectId pack_iri(const std::string& str);
    static ObjectId pack_string_simple(const std::string& str);
    static ObjectId pack_string_xsd(const std::string& str);
    static ObjectId pack_string_lang(uint16_t lang, const std::string& str);
    static ObjectId pack_string_datatype(uint16_t dt, const std::string& str);
    static ObjectId pack_int(int64_t i);
    static ObjectId pack_decimal(Decimal dec);
    static ObjectId pack_float(float flt);
    static ObjectId pack_double(double dbl);

    static int64_t     to_integer(ObjectId oid);
    static Decimal     to_decimal(ObjectId oid);
    static float       to_float(ObjectId oid);
    static double      to_double(ObjectId oid);
    static ObjectId    to_boolean(ObjectId oid);

    // Returns a string with the lexical representation of the value
    static std::string to_lexical_str(ObjectId oid);

    static int64_t unpack_positive_int(ObjectId oid);
    static int64_t unpack_negative_int(ObjectId oid);

    static Decimal unpack_decimal_inlined(ObjectId oid);
    static Decimal unpack_decimal_extern_tmp(ObjectId oid);

private:
    static std::string unpack_named_node_inlined(ObjectId oid);
    static std::string unpack_named_node_extern_tmp(ObjectId oid);

    static std::string unpack_string_simple_inlined(ObjectId oid);
    static std::string unpack_string_simple_extern_tmp(ObjectId oid);

    static std::string unpack_string_xsd_inlined(ObjectId oid);
    static std::string unpack_string_xsd_extern_tmp(ObjectId oid);

    static std::pair<uint16_t, std::string> unpack_string_lang_inlined(ObjectId oid);
    static std::pair<uint16_t, std::string> unpack_string_lang_extern_tmp(ObjectId oid);

    static std::pair<uint16_t, std::string> unpack_string_datatype_inlined(ObjectId oid);
    static std::pair<uint16_t, std::string> unpack_string_datatype_extern_tmp(ObjectId oid);

    static std::string unpack_iri_inlined(ObjectId oid);
    static std::string unpack_iri_extern_tmp(ObjectId oid);
};
} // namespace SPARQL
