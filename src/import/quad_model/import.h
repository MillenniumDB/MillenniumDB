#pragma once

#include <iostream>
#include <functional>

#include "base/exceptions.h"
#include "import/inliner.h"
#include "import/disk_vector.h"
#include "import/external_string.h"
#include "import/quad_model/lexer/lexer.h"
#include "import/quad_model/lexer/state.h"
#include "query_optimizer/quad_model/quad_catalog.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Import {
class OnDiskImport {
public:
    OnDiskImport(const std::string& db_folder, size_t buffer_size_in_GB) :
        buffer_size_in_GB   (buffer_size_in_GB),
        db_folder           (db_folder),
        catalog             (QuadCatalog("catalog.dat")),
        declared_nodes      (db_folder + "/tmp_declared_nodes"),
        labels              (db_folder + "/tmp_labels"),
        properties          (db_folder + "/tmp_properties"),
        edges               (db_folder + "/tmp_edges"),
        equal_from_to       (db_folder + "/tmp_equal_from_to"),
        equal_from_type     (db_folder + "/tmp_equal_from_type"),
        equal_to_type       (db_folder + "/tmp_equal_to_type"),
        equal_from_to_type  (db_folder + "/tmp_equal_from_to_type")
    {
        state_transitions = new int[Token::TOTAL_TOKENS*State::TOTAL_STATES];
        create_automata();
    }

    ~OnDiskImport() {
        delete[](state_transitions);
    }

    void start_import(const std::string& input_filename);

private:
    size_t buffer_size_in_GB;

    int* state_transitions;
    std::function<void()> state_funcs[Token::TOTAL_TOKENS * State::TOTAL_STATES];
    Lexer lexer;
    int current_line;

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

    robin_hood::unordered_set<ExternalString> external_strings_set;

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
        id1 = unmasked_id | ObjectId::MASK_ANON;
        if (unmasked_id > catalog.anonymous_nodes_count) {
            catalog.anonymous_nodes_count = unmasked_id;
        }
    }

    void save_first_id_string() {
        ids_stack.clear();
        normalize_string_literal();

        if (lexer.str_len < 8) {
            id1 = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_INLINED;
        } else {
            id1 = get_or_create_external_string_id() | ObjectId::MASK_STRING_EXTERN;
        }
    }

    void save_first_id_iri() {
        ids_stack.clear();
        //TODO: process IRI when supported
    }

    void save_first_id_int() {
        ids_stack.clear();
        id1 = Inliner::inline_int(atoll(lexer.str));
    }

    void save_first_id_float() {
        ids_stack.clear();
        id1 = Inliner::inline_float(atof(lexer.str));
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
            key_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_INLINED;
        } else {
            key_id = get_or_create_external_string_id() | ObjectId::MASK_STRING_EXTERN;
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
        id2 = unmasked_id | ObjectId::MASK_ANON;
        if (unmasked_id > catalog.anonymous_nodes_count) {
            catalog.anonymous_nodes_count = unmasked_id;
        }
    }

    void save_second_id_string() {
        normalize_string_literal();

        if (lexer.str_len < 8) {
            id2 = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_INLINED;
        } else {
            id2 = get_or_create_external_string_id() | ObjectId::MASK_STRING_EXTERN;
        }
    }

    void save_second_id_iri() {
        // TODO: process IRI when supported
    }

    void save_second_id_int() {
        id2 = Inliner::inline_int(atoll(lexer.str));
    }

    void save_second_id_float() {
        id2 = Inliner::inline_float(atof(lexer.str));
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
            label_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_INLINED;
        } else {
            label_id = get_or_create_external_string_id() | ObjectId::MASK_STRING_EXTERN;
        }
        labels.push_back({id1, label_id});
        ++catalog.label2total_count[label_id];
    }

    void add_node_prop_string() {
        uint64_t value_id;

        normalize_string_literal(); // edits lexer.str_len and lexer.str

        if (lexer.str_len < 8) {
            value_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_INLINED;
        } else {
            value_id = get_or_create_external_string_id() | ObjectId::MASK_STRING_EXTERN;
        }
        properties.push_back({id1, key_id, value_id});
    }

    void add_node_prop_int() {
        uint64_t value_id = Inliner::inline_int(atoll(lexer.str));
        properties.push_back({id1, key_id, value_id});
    }

    void add_node_prop_float() {
        uint64_t value_id = Inliner::inline_float(atof(lexer.str));
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
            value_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_INLINED;
        } else {
            value_id = get_or_create_external_string_id() | ObjectId::MASK_STRING_EXTERN;
        }
        properties.push_back({edge_id, key_id, value_id});
    }

    void add_edge_prop_int() {
        uint64_t value_id = Inliner::inline_int(atoll(lexer.str));
        properties.push_back({edge_id, key_id, value_id});
    }

    void add_edge_prop_float() {
        uint64_t value_id = Inliner::inline_float(atof(lexer.str));
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
        std::cout << "ERROR on line " << current_line << "\n";
    }

private:
    void create_automata();

    void set_transition(int state, int token, int value, std::function<void()> func) {
        state_funcs[State::TOTAL_STATES*state + token] = func;
        state_transitions[State::TOTAL_STATES*state + token] = value;
    }

    int get_transition(int state, int token) {
        state_funcs[State::TOTAL_STATES*state + token]();
        return state_transitions[State::TOTAL_STATES*state + token];
    }

    // modifies contents of lexer.str and lexer.str_len. lexer.str points to the same place
    void normalize_string_literal() {
        char* write_ptr = lexer.str;
        char* read_ptr = write_ptr + 1; // skip first character: '"'

        lexer.str_len -= 2;
        char* end = lexer.str + lexer.str_len + 1;

        while (read_ptr < end) {
            if (*read_ptr == '\\') {
                read_ptr++;
                lexer.str_len--;
                switch (*read_ptr) {
                    case '"':
                        *write_ptr = '"';
                        break;
                    case '\\':
                        *write_ptr = '\\';
                        break;
                    case 'n':
                        *write_ptr = '\n';
                        break;
                    case 't':
                        *write_ptr = '\t';
                        break;
                    case 'r':
                        *write_ptr = '\r';
                        break;
                    // case 'f':
                    //     *write_ptr = '\f';
                    //     break;
                    // case 'b':
                    //     *write_ptr = '\b';
                    //     break;
                    default:
                        *write_ptr = '\\';
                        write_ptr++;
                        *write_ptr = *read_ptr;
                        lexer.str_len++;
                        break;
                }
                read_ptr++;
                write_ptr++;

            } else {
                *write_ptr = *read_ptr;
                read_ptr++;
                write_ptr++;
            }
        }
        *write_ptr = '\0';
    }

    uint64_t get_or_create_external_string_id() {
        // reserve more space if needed
        while (external_strings_end
               + lexer.str_len
               + ExternalString::MIN_PAGE_REMAINING_BYTES
               + ExternalString::MAX_LEN_BYTES
               + 1 >= external_strings_capacity)
        {
            // duplicate buffer
            char* new_external_strings = new char[external_strings_capacity*2];
            std::memcpy(new_external_strings,
                        external_strings,
                        external_strings_capacity);

            external_strings_capacity *= 2;

            delete[] external_strings;
            external_strings = new_external_strings;
            ExternalString::strings = external_strings;
        }


        // encode length
        size_t bytes_for_len = 0;

        auto* ptr = &external_strings[external_strings_end];
        size_t remaining_len = lexer.str_len;

        // TODO: move to string manager?
        while (remaining_len != 0) {
            if (remaining_len <= 127) {
                *ptr = static_cast<char>(remaining_len);
            } else {
                *ptr = static_cast<char>(remaining_len & 0x7FUL) | 0x80;
            }
            remaining_len = remaining_len >> 7;
            bytes_for_len++;
            ptr++;
        }

        // copy string
        std::memcpy(ptr, lexer.str, lexer.str_len);

        ExternalString s(external_strings_end);
        auto found = external_strings_set.find(s);
        if (found == external_strings_set.end()) {
            external_strings_set.insert(s);
            external_strings_end += lexer.str_len + bytes_for_len;

            size_t remaining_in_block = StringManager::STRING_BLOCK_SIZE
                                        - (external_strings_end % StringManager::STRING_BLOCK_SIZE);
            if (remaining_in_block < ExternalString::MIN_PAGE_REMAINING_BYTES) {
                external_strings_end += remaining_in_block;
            }

            return s.offset;
        } else {
            return found->offset;
        }
    }
};
} // namespace Import
