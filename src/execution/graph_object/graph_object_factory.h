#pragma once

#include "base/graph_object/graph_object.h"

class GraphObjectFactory {
public:
    static GraphObject make_null();
    static GraphObject make_int(int64_t);
    static GraphObject make_bool(bool);
    static GraphObject make_float(float);
    static GraphObject make_not_found();
    static GraphObject make_string_external(uint64_t);
    static GraphObject make_named_node_external(uint64_t);
    static GraphObject make_string_inlined(const char c[8]);
    static GraphObject make_named_node_inlined(const char c[8]);
    static GraphObject make_anonymous(uint64_t);
    static GraphObject make_edge(uint64_t);
    static GraphObject make_path(uint64_t);
    static GraphObject make_string_tmp(const std::string&);
    static GraphObject make_named_node_tmp(const std::string&);
    static GraphObject make_iri_external(uint64_t external_id);
    static GraphObject make_iri_inlined(const char* str, uint8_t prefix_id);
    // static GraphObject make_iri_tmp(const std::string& str);
    static GraphObject make_literal_datatype_external(uint64_t external_id);
    static GraphObject make_literal_datatype_inlined(const char* str, uint16_t datatype_id);
    // static GraphObject make_literal_datatype_tmp(const LiteralDatatype& ld);
    static GraphObject make_literal_language_external(uint64_t external_id);
    static GraphObject make_literal_language_inlined(const char* str, uint16_t language_id);
    // static GraphObject make_literal_language_tmp(const LiteralLanguage& ll);
    static GraphObject make_datetime(uint64_t datetime_id);
    static GraphObject make_decimal_external(uint64_t external_id);
    static GraphObject make_decimal_inlined(uint64_t decimal_id);
    // static GraphObject make_decimal_tmp(const std::string& str);
};
