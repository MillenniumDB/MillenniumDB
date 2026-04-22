#pragma once

#include <cstdint>
#include <string>

#include "graph_models/common/conversions.h" // IWYU pragma: export

namespace GQL { namespace Conversions {

using namespace Common::Conversions;

static constexpr uint64_t LAST_TMP_ID = ObjectId::MASK_LITERAL_TAG;

static constexpr uint64_t TMP_SHIFT = 44;

void print_string(ObjectId oid, std::ostream&);
size_t print_string(ObjectId oid, char* out);

void print_path_node(std::ostream& os, ObjectId node_id);
void print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse);

ObjectId pack_edge_label(const std::string& label);
ObjectId pack_node_label(const std::string& label);

ObjectId pack_node_property(const std::string& key);
ObjectId pack_edge_property(const std::string& key);

void print_path(std::ostream& os, ObjectId oid);
ObjectId pack_path(const std::vector<ObjectId>& oid_list);
void unpack_path(ObjectId oid, std::vector<ObjectId>& out);

std::ostream& debug_print(std::ostream& os, ObjectId oid);

ObjectId to_boolean(ObjectId oid);

// Returns a string with the lexical representation of the value
std::string to_lexical_str(ObjectId oid);

std::ostream& debug_print(std::ostream& os, ObjectId oid);

}} // namespace GQL::Conversions
