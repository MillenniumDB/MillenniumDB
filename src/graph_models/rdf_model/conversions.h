#pragma once

#include <cstdint>
#include <string>

#include "graph_models/common/conversions.h" // IWYU pragma: export
#include "graph_models/rdf_model/rdf_object_id.h" // IWYU pragma: export

namespace SPARQL { namespace Conversions {

using namespace Common::Conversions;

// special value for lang and datatypes
// that indicates the lang/dtt is inlined in the string
// example: "external_string@tmp-lang"
//          "external_string^tmp-datatype"
static constexpr uint64_t LAST_TMP_ID = ObjectId::MASK_LITERAL_TAG;

static constexpr uint64_t TMP_SHIFT = 44;

// All unpacks assumes type were checked before calling
uint64_t unpack_blank(ObjectId oid);
std::string unpack_iri(ObjectId oid);
std::string unpack_string(ObjectId oid);

uint64_t get_datatype_id(const std::string& datatype);
uint64_t get_language_id(const std::string& lang);

// returns <lang, str>
std::pair<std::string, std::string> unpack_string_lang(ObjectId oid);

// returns <datatype, str>
std::pair<std::string, std::string> unpack_string_datatype(ObjectId oid);

void print_string(ObjectId oid, std::ostream&);
void print_iri(ObjectId oid, std::ostream&);
void print_path_node(std::ostream& os, ObjectId node_id);
void print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse);

size_t print_iri(ObjectId oid, char* out);
size_t print_iri_suffix(ObjectId oid, char* out);
size_t print_string(ObjectId oid, char* out);

// Doesn't print the language
void print_string_lang(ObjectId oid, std::ostream&);
size_t print_string_lang(ObjectId oid, char* out);

// Doesn't print the datatype
void print_string_datatype(ObjectId oid, std::ostream&);
size_t print_string_datatype(ObjectId oid, char* out);

ObjectId string_simple_to_xsd(ObjectId oid);

inline constexpr ObjectId pack_blank_inline(uint64_t id)
{
    return ObjectId(ObjectId::MASK_ANON_INLINED | id);
}

inline constexpr ObjectId pack_blank_tmp(uint64_t blank_id)
{
    return ObjectId(blank_id | ObjectId::MASK_ANON_TMP);
}

ObjectId pack_iri_inline(const char* str, uint_fast8_t prefix_id);
ObjectId pack_string_simple_inline(const char* str);
ObjectId pack_string_xsd_inline(const char* str);
ObjectId pack_string_datatype_inline(uint64_t datatype_id, const char* str);
ObjectId pack_string_lang_inline(uint64_t lang_id, const char* str);

ObjectId pack_string_lang(const std::string& lang, const std::string& str);
ObjectId pack_string_datatype(const std::string& dt, const std::string& str);
ObjectId try_pack_string_datatype(const std::string& dt, const std::string& str);
ObjectId try_pack_xsd_datatype(const std::string& dt, const std::string& str);
ObjectId try_pack_mdb_datatype(const std::string& dt, const std::string& str);
ObjectId try_pack_integer(const std::string& dt, const std::string& str);

constexpr ObjectId pack_empty_string()
{
    return ObjectId(ObjectId::MASK_STRING_SIMPLE_INLINED);
}

ObjectId pack_iri(const std::string& str);
ObjectId pack_string_simple(const std::string& str);
ObjectId pack_string_xsd(const std::string& str);

// Returns a string with the lexical representation of the value
std::string to_lexical_str(ObjectId oid);

ObjectId to_boolean(ObjectId oid);

std::ostream& debug_print(std::ostream& os, ObjectId object_id);

size_t print_iri_uuid_lower(ObjectId oid, char* out);
size_t print_iri_uuid_upper(ObjectId oid, char* out);
size_t print_tmp_iri_uuid_lower(ObjectId oid, char* out);
size_t print_tmp_iri_uuid_upper(ObjectId oid, char* out);

size_t print_iri_hex_lower(ObjectId oid, char* out);
size_t print_tmp_iri_hex_lower(ObjectId oid, char* out);
size_t print_iri_hex_upper(ObjectId oid, char* out);
size_t print_tmp_iri_hex_upper(ObjectId oid, char* out);

}} // namespace SPARQL::Conversions
