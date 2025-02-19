#pragma once

#include <cctype>
#include <functional>
#include <iostream>

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>

#include "graph_models/common/conversions.h"
#include "graph_models/inliner.h"
#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/quad_model/quad_catalog.h"
#include "import/disk_vector.h"
#include "import/exceptions.h"
#include "import/external_string.h"
#include "import/quad_model/lexer/state.h"
#include "import/quad_model/lexer/token.h"
#include "import/quad_model/lexer/tokenizer.h"
#include "macros/aligned_alloc.h"
#include "misc/istream.h"
#include "misc/unicode_escape.h"

namespace Import { namespace QuadModel {
class OnDiskImport {
public:
    OnDiskImport(const std::string& db_folder, uint64_t buffer_size) :
        buffer_size        (buffer_size),
        db_folder          (db_folder),
        catalog            (QuadCatalog("catalog.dat")),
        declared_nodes     (db_folder + "/tmp_declared_nodes"),
        labels             (db_folder + "/tmp_labels"),
        properties         (db_folder + "/tmp_properties"),
        edges              (db_folder + "/tmp_edges"),
        equal_from_to      (db_folder + "/tmp_equal_from_to"),
        equal_from_type    (db_folder + "/tmp_equal_from_type"),
        equal_to_type      (db_folder + "/tmp_equal_to_type"),
        equal_from_to_type (db_folder + "/tmp_equal_from_to_type")
    {
        state_transitions = new int[Token::TOTAL_TOKENS*State::TOTAL_STATES];
        create_automata();
    }

    ~OnDiskImport() {
        delete[](state_transitions);
    }

    void start_import(MDBIstream& in);

private:
    uint64_t buffer_size;

    int* state_transitions;
    std::function<void()> state_funcs[Token::TOTAL_TOKENS * State::TOTAL_STATES];
    MQLTokenizer lexer;
    int current_line;

    uint64_t parsing_errors = 0;

    uint64_t id1;
    uint64_t id2;
    uint64_t type_id;
    uint64_t edge_id;
    uint64_t key_id;
    uint64_t edge_count = 0;
    std::vector<uint64_t> ids_stack;

    // true: right, false: left
    bool direction;

    std::string db_folder;
    QuadCatalog catalog;

    DiskVector<1> declared_nodes;
    DiskVector<2> labels;
    DiskVector<3> properties;
    DiskVector<4> edges;
    DiskVector<3> equal_from_to;
    DiskVector<3> equal_from_type;
    DiskVector<3> equal_to_type;
    DiskVector<2> equal_from_to_type;

    boost::unordered_flat_set<ExternalString, std::hash<ExternalString>> external_strings_set;

    char*    external_strings;
    uint64_t external_strings_capacity;
    uint64_t external_strings_end;

    void do_nothing() { }
    void set_left_direction() { direction = false; }
    void set_right_direction() { direction = true; }

    void save_first_id_identifier() {
        ids_stack.clear();
        if (lexer.str_len < 8) {
            id1 = Inliner::inline_string(lexer.str) | ObjectId::MASK_NAMED_NODE_INLINED;
        } else {
            id1 = get_or_create_external_string_id() | ObjectId::MASK_NAMED_NODE_EXTERN;
        }
    }

    void save_first_id_anon() {
        ids_stack.clear();
        uint64_t unmasked_id = std::stoull(lexer.str + 2);
        id1 = unmasked_id | ObjectId::MASK_ANON_INLINED;
    }

    void save_first_id_string() {
        ids_stack.clear();
        normalize_string_literal();

        if (lexer.str_len < 8) {
            id1 = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            id1 = get_or_create_external_string_id() | ObjectId::MASK_STRING_SIMPLE_EXTERN;
        }
    }

    int64_t try_parse_int(char* c_str) {
        return Common::Conversions::pack_int(atoll(c_str)).id;
    }

    int64_t try_parse_float(char* c_str) {
        return Common::Conversions::pack_float(atof(c_str)).id;
    }

    void save_first_id_int() {
        ids_stack.clear();
        id1 = try_parse_int(lexer.str);
    }

    void save_first_id_float() {
        ids_stack.clear();
        id1 = try_parse_float(lexer.str);
    }

    void save_first_id_true() {
        ids_stack.clear();
        id1 = ObjectId::MASK_BOOL | 0x01;
    }

    void save_first_id_false() {
        ids_stack.clear();
        id1 = ObjectId::MASK_BOOL | 0x00;
    }

    void save_first_id_implicit() {
        if (ids_stack.size() == 0) {
            throw ImportException("[line " + std::to_string(current_line)
                + "] can't use implicit edge on undefined object");
        }
        else if (lexer.str_len < ids_stack.size()) {
            id1 = ids_stack[lexer.str_len-1];
            ids_stack.resize(lexer.str_len);
            ids_stack.push_back(edge_id);
        }
        else if (lexer.str_len == ids_stack.size()) {
            id1 = ids_stack[lexer.str_len-1];
            ids_stack.push_back(edge_id);
        }
        else {
            throw ImportException("[line " + std::to_string(current_line)
                + "] undefined level of implicit edge");
        }
    }

    void save_edge_type() {
        if (lexer.str_len < 8) {
            type_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_NAMED_NODE_INLINED;
        } else {
            type_id = get_or_create_external_string_id() | ObjectId::MASK_NAMED_NODE_EXTERN;
        }
        ++catalog.type2total_count[type_id];

        edge_id = ++edge_count | ObjectId::MASK_EDGE;
        if (direction) {
            edges.push_back({id1, id2, type_id, edge_id});
        } else {
            edges.push_back({id2, id1, type_id, edge_id});
        }

        if (id1 == id2) {
            equal_from_to.push_back({id1, type_id, edge_id});

            if (id1 == type_id) {
                equal_from_to_type.push_back({id1, edge_id});
            }
        }
        if (id1 == type_id) {
            if (direction) {
                equal_from_type.push_back({id1, id2, edge_id});
            } else {
                equal_to_type.push_back({id1, id2, edge_id});
            }
        }
        if (id2 == type_id) {
            if (direction) {
                equal_to_type.push_back({id1, id2, edge_id});
            } else {
                equal_from_type.push_back({id1, id2, edge_id});
            }
        }

        ids_stack.push_back(edge_id);
    }

    void save_prop_key() {
        if (lexer.str_len < 8) {
            key_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            key_id = get_or_create_external_string_id() | ObjectId::MASK_STRING_SIMPLE_EXTERN;
        }
    }

    void save_second_id_identifier() {
        if (lexer.str_len < 8) {
            id2 = Inliner::inline_string(lexer.str) | ObjectId::MASK_NAMED_NODE_INLINED;
        } else {
            id2 = get_or_create_external_string_id() | ObjectId::MASK_NAMED_NODE_EXTERN;
        }
    }

    void save_second_id_anon() {
        // delete first 2 characters: '_a'
        uint64_t unmasked_id = std::stoull(lexer.str + 2);
        id2 = unmasked_id | ObjectId::MASK_ANON_INLINED;
    }

    void save_second_id_string() {
        normalize_string_literal();

        if (lexer.str_len < 8) {
            id2 = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            id2 = get_or_create_external_string_id() | ObjectId::MASK_STRING_SIMPLE_EXTERN;
        }
    }

    void save_second_id_int() {
        id2 = try_parse_int(lexer.str);
    }

    void save_second_id_float() {
        id2 = try_parse_float(lexer.str);
    }

    void save_second_id_true() {
        id2 = ObjectId::MASK_BOOL | 0x01;
    }

    void save_second_id_false() {
        id2 = ObjectId::MASK_BOOL | 0x00;
    }

    void add_node_label() {
        uint64_t label_id;
        if (lexer.str_len < 8) {
            label_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            label_id = get_or_create_external_string_id() | ObjectId::MASK_STRING_SIMPLE_EXTERN;
        }
        labels.push_back({id1, label_id});
        ++catalog.label2total_count[label_id];
    }

    void add_node_prop_datatype() {
        add_prop_datatype(id1);
    }

    void add_edge_prop_datatype() {
        add_prop_datatype(edge_id);
    }

    void add_prop_datatype(uint64_t obj_id) {
        uint64_t value_id;

        // we have something like: `datatype("string")`
        // parse datatype name
        char* datatype_beg = lexer.str;
        char* datatype_end = lexer.str;
        while (isalpha(*datatype_end)) {
            datatype_end++;
        }
        *datatype_end = '\0';

        char* str_value_end = lexer.str + (lexer.str_len-1);
        lexer.str = datatype_end + 1;
        while (*lexer.str != '"') {
            lexer.str++;
        }

        // it may have whitespaces `datatype("string"  )` so we iterate
        while (*str_value_end != '"') {
            str_value_end--;
        }
        lexer.str_len = (str_value_end-lexer.str)+1;

        // we edited lexer.str_len and lexer.str to point correctly at the datatype (considering quotes)
        normalize_string_literal(); // edits lexer.str_len and lexer.str

        if (strcmp(datatype_beg, "dateTime") == 0) {
            value_id = DateTime::from_dateTime(lexer.str);
            if (value_id == ObjectId::NULL_ID) {
                parsing_errors++;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << " invalid dateTime: " << lexer.str << "\n";
                return;
            }
        } else if (strcmp(datatype_beg, "date") == 0) {
            value_id = DateTime::from_date(lexer.str);
            if (value_id == ObjectId::NULL_ID) {
                parsing_errors++;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << "invalid date: " << lexer.str << "\n";
                return;
            }
        } else if (strcmp(datatype_beg, "time") == 0) {
            value_id = DateTime::from_time(lexer.str);
            if (value_id == ObjectId::NULL_ID) {
                parsing_errors++;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << "invalid time: " << lexer.str << "\n";
                return;
            }
        } else if (strcmp(datatype_beg, "dateTimeStamp") == 0) {
            value_id = DateTime::from_dateTimeStamp(lexer.str);
            if (value_id == ObjectId::NULL_ID) {
                parsing_errors++;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << "invalid dateTimeStamp: " << lexer.str << "\n";
                return;
            }
        } else {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << "unknown datatype: " << datatype_beg << "\n";
            return;
        }

        properties.push_back({obj_id, key_id, value_id});
    }

    void add_node_prop_string() {
        uint64_t value_id;

        normalize_string_literal(); // edits lexer.str_len and lexer.str

        if (lexer.str_len < 8) {
            value_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            value_id = get_or_create_external_string_id() | ObjectId::MASK_STRING_SIMPLE_EXTERN;
        }
        properties.push_back({id1, key_id, value_id});
    }

    void add_node_prop_int() {
        uint64_t value_id = try_parse_int(lexer.str);
        properties.push_back({id1, key_id, value_id});
    }

    void add_node_prop_float() {
        uint64_t value_id = try_parse_float(lexer.str);
        properties.push_back({id1, key_id, value_id});
    }

    void add_node_prop_true() {
        uint64_t value_id = ObjectId::MASK_BOOL | 0x01;
        properties.push_back({id1, key_id, value_id});
    }

    void add_node_prop_false() {
        uint64_t value_id = ObjectId::MASK_BOOL | 0x00;
        properties.push_back({id1, key_id, value_id});
    }

    void add_edge_prop_string() {
        uint64_t value_id;

        normalize_string_literal(); // edits lexer.str_len and lexer.str

        if (lexer.str_len < 8) {
            value_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            value_id = get_or_create_external_string_id() | ObjectId::MASK_STRING_SIMPLE_EXTERN;
        }
        properties.push_back({edge_id, key_id, value_id});
    }

    void add_edge_prop_int() {
        uint64_t value_id = try_parse_int(lexer.str);
        properties.push_back({edge_id, key_id, value_id});
    }

    void add_edge_prop_float() {
        uint64_t value_id = try_parse_float(lexer.str);
        properties.push_back({edge_id, key_id, value_id});
    }

    void add_edge_prop_true() {
        uint64_t value_id = ObjectId::MASK_BOOL | 0x01;
        properties.push_back({edge_id, key_id, value_id});
    }

    void add_edge_prop_false() {
        uint64_t value_id = ObjectId::MASK_BOOL | 0x00;
        properties.push_back({edge_id, key_id, value_id});
    }

    void finish_wrong_line() {
        current_line++;
    }

    void finish_node_line() {
        declared_nodes.push_back({id1});
        ids_stack.push_back(id1);
        current_line++;
    }

    void finish_edge_line() {
        current_line++;
    }

    void print_error() {
        parsing_errors++;
        std::cout << "ERROR on line " << current_line << "\n";
    }

private:
    void create_automata();

    void set_transition(int state, int token, int value, std::function<void()> func) {
        state_funcs[State::TOTAL_STATES*state + token] = func;
        state_transitions[State::TOTAL_STATES*state + token] = value;
    }

    int get_transition(int state, int token) {
        // try {
        //     state_funcs[State::TOTAL_STATES*state + token]();
        //     return state_transitions[State::TOTAL_STATES*state + token];
        // }
        // catch (std::exception& e) {
        //     parsing_errors++;
        //     std::cout << "ERROR on line " << current_line << "\n";
        //     std::cout << e.what() << "\n";
        //     return State::WRONG_LINE;
        // }
        state_funcs[State::TOTAL_STATES*state + token]();
        return state_transitions[State::TOTAL_STATES*state + token];
    }

    // modifies contents of lexer.str and lexer.str_len. lexer.str points to the same place
    void normalize_string_literal() {
        char* write_ptr = lexer.str;
        char* read_ptr = write_ptr + 1; // skip first character: '"'

        lexer.str_len -= 2;
        char* end = lexer.str + lexer.str_len + 1;

        UnicodeEscape::normalize_string(read_ptr, write_ptr, end, lexer.str_len);
    }

    uint64_t get_or_create_external_string_id() {
        // reserve more space if needed
        while (external_strings_end
               + lexer.str_len
               + StringManager::MIN_PAGE_REMAINING_BYTES
               + StringManager::MAX_LEN_BYTES
               + 1 >= external_strings_capacity)
        {
            // TODO: use pending strings
            // duplicate buffer
            char* new_external_strings = reinterpret_cast<char*>(
                MDB_ALIGNED_ALLOC(external_strings_capacity * 2)
            );
            std::memcpy(new_external_strings,
                        external_strings,
                        external_strings_capacity);

            external_strings_capacity *= 2;

            MDB_ALIGNED_FREE(external_strings);
            external_strings = new_external_strings;
            ExternalString::strings = external_strings;
        }

        // encode length
        auto bytes_for_len = StringManager::write_encoded_strlen(
            &external_strings[external_strings_end],
            lexer.str_len);

        // copy string
        std::memcpy(
            &external_strings[external_strings_end] + bytes_for_len,
            lexer.str,
            lexer.str_len);

        ExternalString s(external_strings_end);
        auto found = external_strings_set.find(s);
        if (found == external_strings_set.end()) {
            external_strings_set.insert(s);
            external_strings_end += lexer.str_len + bytes_for_len;

            size_t remaining_in_block = StringManager::BLOCK_SIZE
                                        - (external_strings_end % StringManager::BLOCK_SIZE);
            if (remaining_in_block < StringManager::MIN_PAGE_REMAINING_BYTES) {
                external_strings_end += remaining_in_block;
            }

            return s.offset;
        } else {
            return found->offset;
        }
    }
};
}} // namespace Import::QuadModel
