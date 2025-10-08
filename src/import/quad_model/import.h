#pragma once

#include <cctype>
#include <functional>
#include <iostream>

#include <boost/unordered/unordered_flat_set.hpp>

#include "graph_models/common/conversions.h"
#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/inliner.h"
#include "graph_models/quad_model/quad_catalog.h"
#include "import/disk_vector.h"
#include "import/exceptions.h"
#include "import/external_helper.h"
#include "import/quad_model/lexer/state.h"
#include "import/quad_model/lexer/token.h"
#include "import/quad_model/lexer/tokenizer.h"
#include "misc/istream.h"
#include "misc/unicode_escape.h"
#include "storage/index/lists/list_encoder.h"

namespace Import { namespace QuadModel {
class OnDiskImport {
public:
    static constexpr char PENDING_DECLARED_NODES_FILENAME_PREFIX[] = "tmp_pending_declared_nodes";
    static constexpr char PENDING_LABELS_FILENAME_PREFIX[] = "tmp_pending_labels";
    static constexpr char PENDING_PROPERTIES_FILENAME_PREFIX[] = "tmp_pending_properties";
    static constexpr char PENDING_EDGES_FILENAME_PREFIX[] = "tmp_pending_edges";

    OnDiskImport(const std::string& db_folder, uint64_t strings_buffer_size, uint64_t tensors_buffer_size) :
        strings_buffer_size(strings_buffer_size),
        tensors_buffer_size(tensors_buffer_size),
        db_folder(db_folder),
        catalog(QuadCatalog("catalog.dat")),
        declared_nodes(db_folder + "/tmp_declared_nodes"),
        labels(db_folder + "/tmp_labels"),
        properties(db_folder + "/tmp_properties"),
        edges(db_folder + "/tmp_edges"),
        equal_from_to(db_folder + "/tmp_equal_from_to"),
        equal_from_type(db_folder + "/tmp_equal_from_type"),
        equal_to_type(db_folder + "/tmp_equal_to_type"),
        equal_from_to_type(db_folder + "/tmp_equal_from_to_type")
    {
        state_transitions = new int[Token::TOTAL_TOKENS * State::TOTAL_STATES];
        create_automata();
        list_buffer = new char[StringManager::MAX_STRING_SIZE];
    }

    ~OnDiskImport()
    {
        delete[] (state_transitions);
        delete[] list_buffer;
    }

    void start_import(MDBIstream& in);

private:
    uint64_t strings_buffer_size;
    uint64_t tensors_buffer_size;

    int* state_transitions;
    std::function<void()> state_funcs[Token::TOTAL_TOKENS * State::TOTAL_STATES];
    MQLTokenizer lexer;
    int current_line;
    int current_state;

    // we use a stack to represent nested lists
    std::stack<std::vector<ObjectId>> lists_stack;

    // buffer used to encode lists
    char* list_buffer;

    uint64_t parsing_errors = 0;

    uint64_t id1;
    uint64_t id2;
    uint64_t type_id;
    uint64_t edge_id;
    uint64_t key_id;
    uint64_t value_id;
    uint64_t label_id;
    uint64_t edge_count = 0;
    uint64_t max_anon_seen = 0;
    std::vector<uint64_t> ids_stack;

    // true: right, false: left
    bool direction;

    std::string db_folder;
    QuadCatalog catalog;

    std::unique_ptr<DiskVector<1>> pending_declared_nodes;
    std::unique_ptr<DiskVector<2>> pending_labels;
    std::unique_ptr<DiskVector<3>> pending_properties;
    std::unique_ptr<DiskVector<4>> pending_edges;

    DiskVector<1> declared_nodes;
    DiskVector<2> labels;
    DiskVector<3> properties;
    DiskVector<4> edges;
    DiskVector<3> equal_from_to;
    DiskVector<3> equal_from_type;
    DiskVector<3> equal_to_type;
    DiskVector<2> equal_from_to_type;

    // manager writing bytes to disk in a buffered manner
    std::unique_ptr<ExternalHelper> external_helper;

    void do_nothing() { }

    void set_left_direction()
    {
        direction = false;
    }

    void set_right_direction()
    {
        direction = true;
    }

    void save_first_id_identifier()
    {
        ids_stack.clear();
        if (lexer.str_len < 8) {
            id1 = Inliner::inline_string(lexer.str) | ObjectId::MASK_NAMED_NODE_INLINED;
        } else {
            id1 = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                | ObjectId::MASK_NAMED_NODE;
        }
    }

    void save_first_id_anon()
    {
        ids_stack.clear();
        // ignore first 2 characters: '_a'
        uint64_t unmasked_id = std::stoull(lexer.str + 2);
        id1 = unmasked_id | ObjectId::MASK_ANON_INLINED;
        if (unmasked_id > max_anon_seen) {
            max_anon_seen = unmasked_id;
        }
    }

    void save_first_id_string()
    {
        ids_stack.clear();
        normalize_string_literal(lexer.str, &lexer.str_len);

        if (lexer.str_len < 8) {
            id1 = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            id1 = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                | ObjectId::MASK_STRING_SIMPLE;
        }
    }

    int64_t try_parse_int(char* c_str)
    {
        return Common::Conversions::pack_int(atoll(c_str)).id;
    }

    int64_t try_parse_float(char* c_str)
    {
        return Common::Conversions::pack_float(atof(c_str)).id;
    }

    void save_first_id_int()
    {
        ids_stack.clear();
        id1 = try_parse_int(lexer.str);
    }

    void save_first_id_float()
    {
        ids_stack.clear();
        id1 = try_parse_float(lexer.str);
    }

    void save_first_id_true()
    {
        ids_stack.clear();
        id1 = ObjectId::MASK_BOOL | 0x01;
    }

    void save_first_id_false()
    {
        ids_stack.clear();
        id1 = ObjectId::MASK_BOOL | 0x00;
    }

    void save_first_id_implicit()
    {
        if (ids_stack.size() == 0) {
            throw ImportException(
                "[line " + std::to_string(current_line) + "] can't use implicit edge on undefined object"
            );
        } else if (lexer.str_len < ids_stack.size()) {
            id1 = ids_stack[lexer.str_len - 1];
            ids_stack.resize(lexer.str_len);
            ids_stack.push_back(edge_id);
        } else if (lexer.str_len == ids_stack.size()) {
            id1 = ids_stack[lexer.str_len - 1];
            ids_stack.push_back(edge_id);
        } else {
            throw ImportException(
                "[line " + std::to_string(current_line) + "] undefined level of implicit edge"
            );
        }
    }

    void try_save_declared_node()
    {
        if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
            pending_declared_nodes->push_back({ id1 });
        } else {
            declared_nodes.push_back({ id1 });
        }
    }

    // templated to prevent branching while processing pending_edges
    template<bool is_right_direction>
    void try_save_quad()
    {
        if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP || (id2 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
            || (type_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
        {
            if constexpr (is_right_direction) {
                pending_edges->push_back({ id1, id2, type_id, edge_id });
            } else {
                pending_edges->push_back({ id2, id1, type_id, edge_id });
            }
            return;
        }

        if constexpr (is_right_direction) {
            edges.push_back({ id1, id2, type_id, edge_id });
        } else {
            edges.push_back({ id2, id1, type_id, edge_id });
        }

        if (id1 == id2) {
            equal_from_to.push_back({ id1, type_id, edge_id });

            if (id1 == type_id) {
                equal_from_to_type.push_back({ id1, edge_id });
            }
        }

        if (id1 == type_id) {
            if constexpr (is_right_direction) {
                equal_from_type.push_back({ id1, id2, edge_id });
            } else {
                equal_to_type.push_back({ id1, id2, edge_id });
            }
        }

        if (id2 == type_id) {
            if constexpr (is_right_direction) {
                equal_to_type.push_back({ id1, id2, edge_id });
            } else {
                equal_from_type.push_back({ id1, id2, edge_id });
            }
        }
    }

    void try_save_label()
    {
        if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
            || (label_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
        {
            pending_labels->push_back({ id1, label_id });
        } else {
            labels.push_back({ id1, label_id });
        }
    }

    void try_save_property(uint64_t obj_id)
    {
        if ((obj_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
            || (key_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
            || (value_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
        {
            pending_properties->push_back({ obj_id, key_id, value_id });
        } else {
            properties.push_back({ obj_id, key_id, value_id });
        }
    }

    void save_edge_type()
    {
        if (lexer.str_len < 8) {
            type_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_NAMED_NODE_INLINED;
        } else {
            type_id = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                    | ObjectId::MASK_NAMED_NODE;
        }

        edge_id = edge_count++ | ObjectId::MASK_EDGE;
        ids_stack.push_back(edge_id);

        if (direction) {
            try_save_quad<true>();
        } else {
            try_save_quad<false>();
        }
    }

    void save_prop_key()
    {
        if (lexer.str_len < 8) {
            key_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            key_id = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                   | ObjectId::MASK_STRING_SIMPLE;
        }
    }

    void save_second_id_identifier()
    {
        if (lexer.str_len < 8) {
            id2 = Inliner::inline_string(lexer.str) | ObjectId::MASK_NAMED_NODE_INLINED;
        } else {
            id2 = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                | ObjectId::MASK_NAMED_NODE;
        }
    }

    void save_second_id_anon()
    {
        // ignore first 2 characters: '_a'
        uint64_t unmasked_id = std::stoull(lexer.str + 2);
        id2 = unmasked_id | ObjectId::MASK_ANON_INLINED;
        if (unmasked_id > max_anon_seen) {
            max_anon_seen = unmasked_id;
        }
    }

    void save_second_id_string()
    {
        normalize_string_literal(lexer.str, &lexer.str_len);

        if (lexer.str_len < 8) {
            id2 = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            id2 = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                | ObjectId::MASK_STRING_SIMPLE;
        }
    }

    void save_second_id_int()
    {
        id2 = try_parse_int(lexer.str);
    }

    void save_second_id_float()
    {
        id2 = try_parse_float(lexer.str);
    }

    void save_second_id_true()
    {
        id2 = ObjectId::MASK_BOOL | 0x01;
    }

    void save_second_id_false()
    {
        id2 = ObjectId::MASK_BOOL | 0x00;
    }

    void add_node_label()
    {
        if (lexer.str_len < 8) {
            label_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            label_id = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                     | ObjectId::MASK_STRING_SIMPLE;
        }

        try_save_label();
    }

    void add_node_prop_datatype()
    {
        parse_prop_datatype(lexer.str, lexer.str_len);
        try_save_property(id1);
    }

    void add_edge_prop_datatype()
    {
        parse_prop_datatype(lexer.str, lexer.str_len);
        try_save_property(edge_id);
    }

    // parse datatype and store it in value_id
    void parse_prop_datatype(char* typed_str, uint64_t typed_str_len)
    {
        // we have something like: `datatype("string")`
        // parse datatype name
        char* datatype_beg = typed_str;
        char* datatype_end = typed_str;
        while (isalpha(*datatype_end)) {
            datatype_end++;
        }
        *datatype_end = '\0';

        char* str_value_end = typed_str + (typed_str_len - 1);
        typed_str = datatype_end + 1;
        while (*typed_str != '"') {
            typed_str++;
        }

        // it may have whitespaces `datatype("string"  )` so we iterate
        while (*str_value_end != '"') {
            str_value_end--;
        }
        typed_str_len = (str_value_end - typed_str) + 1;

        // we edited typed_str_len and typed_str to point correctly at the datatype (considering quotes)
        normalize_string_literal(typed_str, &typed_str_len);

        if (strcmp(datatype_beg, "dateTime") == 0) {
            value_id = DateTime::from_dateTime(typed_str);
            if (value_id == ObjectId::NULL_ID) {
                parsing_errors++;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << " invalid dateTime: " << typed_str << "\n";
                return;
            }
        } else if (strcmp(datatype_beg, "date") == 0) {
            value_id = DateTime::from_date(typed_str);
            if (value_id == ObjectId::NULL_ID) {
                parsing_errors++;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << "invalid date: " << typed_str << "\n";
                return;
            }
        } else if (strcmp(datatype_beg, "time") == 0) {
            value_id = DateTime::from_time(typed_str);
            if (value_id == ObjectId::NULL_ID) {
                parsing_errors++;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << "invalid time: " << typed_str << "\n";
                return;
            }
        } else if (strcmp(datatype_beg, "dateTimeStamp") == 0) {
            value_id = DateTime::from_dateTimeStamp(typed_str);
            if (value_id == ObjectId::NULL_ID) {
                parsing_errors++;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << "invalid dateTimeStamp: " << typed_str << "\n";
                return;
            }
        } else if (strcmp(datatype_beg, "tensorFloat") == 0) {
            value_id = get_tensor_id<float>(typed_str);
            if (value_id == ObjectId::NULL_ID) {
                ++parsing_errors;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << "invalid tensorFloat: " << typed_str << "\n";
            }
        } else if (strcmp(datatype_beg, "tensorDouble") == 0) {
            value_id = get_tensor_id<double>(typed_str);
            if (value_id == ObjectId::NULL_ID) {
                ++parsing_errors;
                std::cout << "ERROR on line " << current_line << ", ";
                std::cout << "invalid tensorDouble: " << typed_str << "\n";
            }
        } else {
            parsing_errors++;
            std::cout << "ERROR on line " << current_line << ", ";
            std::cout << "unknown datatype: " << datatype_beg << "\n";
            return;
        }
    }

    template<typename T>
    uint64_t get_tensor_id(std::string_view str)
    {
        bool error;
        const auto tensor = tensor::Tensor<T>::from_literal(str, &error);
        if (error) {
            return ObjectId::NULL_ID;
        }

        const auto bytes = reinterpret_cast<const char*>(tensor.data());
        const auto num_bytes = sizeof(T) * tensor.size();
        return tensor::Tensor<T>::get_subtype()
             | external_helper->get_or_create_external_tensor_id(bytes, num_bytes);
    }

    void add_node_prop_string()
    {
        normalize_string_literal(lexer.str, &lexer.str_len);

        if (lexer.str_len < 8) {
            value_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            value_id = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                     | ObjectId::MASK_STRING_SIMPLE;
        }

        try_save_property(id1);
    }

    void add_node_prop_int()
    {
        value_id = try_parse_int(lexer.str);
        try_save_property(id1);
    }

    void add_node_prop_float()
    {
        value_id = try_parse_float(lexer.str);
        try_save_property(id1);
    }

    void add_node_prop_true()
    {
        value_id = ObjectId::MASK_BOOL | 0x01;
        try_save_property(id1);
    }

    void add_node_prop_false()
    {
        value_id = ObjectId::MASK_BOOL | 0x00;
        try_save_property(id1);
    }

    void add_edge_prop_string()
    {
        normalize_string_literal(lexer.str, &lexer.str_len);

        if (lexer.str_len < 8) {
            value_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            value_id = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                     | ObjectId::MASK_STRING_SIMPLE;
        }

        try_save_property(edge_id);
    }

    void add_edge_prop_int()
    {
        value_id = try_parse_int(lexer.str);
        try_save_property(edge_id);
    }

    void add_edge_prop_float()
    {
        value_id = try_parse_float(lexer.str);
        try_save_property(edge_id);
    }

    void add_edge_prop_true()
    {
        value_id = ObjectId::MASK_BOOL | 0x01;
        try_save_property(edge_id);
    }

    void add_edge_prop_false()
    {
        value_id = ObjectId::MASK_BOOL | 0x00;
        try_save_property(edge_id);
    }

    void init_list()
    {
        lists_stack.emplace();
    }

    void add_list_value_false()
    {
        ObjectId value = Common::Conversions::pack_bool(false);
        lists_stack.top().push_back(value);
    }

    void add_list_value_true()
    {
        ObjectId value = Common::Conversions::pack_bool(true);
        lists_stack.top().push_back(value);
    }

    void add_list_value_integer()
    {
        int64_t integer = try_parse_int(lexer.str);
        lists_stack.top().emplace_back(integer);
    }

    void add_list_value_float()
    {
        int64_t value = try_parse_float(lexer.str);
        lists_stack.top().emplace_back(value);
    }

    void add_list_value_string()
    {
        normalize_string_literal(lexer.str, &lexer.str_len);

        uint64_t str_id;
        if (lexer.str_len < 8) {
            str_id = Inliner::inline_string(lexer.str) | ObjectId::MASK_STRING_SIMPLE_INLINED;
        } else {
            str_id = external_helper->get_or_create_external_string_id(lexer.str, lexer.str_len)
                   | ObjectId::MASK_STRING;
        }

        lists_stack.top().emplace_back(str_id);
    }

    void add_list_value_typed_string()
    {
        parse_prop_datatype(lexer.str, lexer.str_len);
        lists_stack.top().emplace_back(value_id);
    }

    void save_node_list()
    {
        std::vector<ObjectId> current_list = lists_stack.top();
        uint64_t encoded_size = ListEncoder::encode(current_list, list_buffer);
        lists_stack.pop();

        uint64_t list_id = external_helper->get_or_create_external_string_id(list_buffer, encoded_size)
                         | ObjectId::MASK_LIST;

        // if there is a list in the stack, then this list is nested and we do not store the property yet
        if (!lists_stack.empty()) {
            current_state = EXPECT_NODE_LIST_ELEMENT;
            lists_stack.top().emplace_back(list_id);
            return;
        }

        if ((id1 & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
            || (list_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
        {
            pending_properties->push_back({ id1, key_id, list_id });
        } else {
            properties.push_back({ id1, key_id, list_id });
        }
    }

    void save_edge_list()
    {
        std::vector<ObjectId> current_list = lists_stack.top();
        uint64_t encoded_size = ListEncoder::encode(current_list, list_buffer);
        lists_stack.pop();

        uint64_t list_id = external_helper->get_or_create_external_string_id(list_buffer, encoded_size)
                         | ObjectId::MASK_LIST;

        // if there is a list in the stack, then this list is nested and we do not store the property yet
        if (!lists_stack.empty()) {
            current_state = EXPECT_EDGE_LIST_ELEMENT;
            lists_stack.top().emplace_back(list_id);
            return;
        }

        if ((edge_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
            || (list_id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
        {
            pending_properties->push_back({ edge_id, key_id, list_id });
        } else {
            properties.push_back({ edge_id, key_id, list_id });
        }
    }

    void finish_wrong_line()
    {
        current_line++;
    }

    void finish_node_line()
    {
        ids_stack.push_back(id1);
        current_line++;

        try_save_declared_node();
    }

    void finish_edge_line()
    {
        current_line++;
    }

    void print_error()
    {
        parsing_errors++;
        std::cout << "ERROR on line " << current_line << "\n";
    }

    // processes a pending file by iterations, until no more pending tuples are available
    // the size of the tuples and a resolve+save function must be provided.
    // pending_vector is passed-by-reference in order to keep it valid when calling the
    // resolve_and_save_func
    template<std::size_t N, typename ResolveAndSaveFunc>
    inline void process_pending(
        std::unique_ptr<DiskVector<N>>& pending_vector,
        const std::string& name,
        const std::string& pending_filename_prefix,
        ResolveAndSaveFunc resolve_and_save_func
    )
    {
        pending_vector->finish_appends();
        int i = 0;
        while (true) {
            const auto total_pending = pending_vector->get_total_tuples();
            if (total_pending == 0) {
                break;
            }
            std::cout << "pending " + name + ": " << total_pending << std::endl;

            auto old_pending_vector = std::move(pending_vector);
            pending_vector = std::make_unique<DiskVector<N>>(
                db_folder + "/" + pending_filename_prefix + std::to_string(i)
            );
            ++i;

            // advance pending variables for current iteration
            external_helper->advance_pending();
            external_helper->clear_sets();

            old_pending_vector->begin_tuple_iter();
            while (old_pending_vector->has_next_tuple()) {
                const auto& pending_tuple = old_pending_vector->next_tuple();

                resolve_and_save_func(pending_tuple);
            }

            // write out new data
            external_helper->flush_to_disk();
            // close and delete the old pending files
            external_helper->clean_up_old();

            // close and delete old pending file
            pending_vector->finish_appends();
            old_pending_vector->skip_indexing(); // will close and remove file
        }

        // process pending finished, clean up the last pending file
        pending_vector->skip_indexing();
    }

private:
    void create_automata();

    void set_transition(int state, int token, int value, std::function<void()> func)
    {
        state_funcs[State::TOTAL_STATES * state + token] = func;
        state_transitions[State::TOTAL_STATES * state + token] = value;
    }

    void get_transition(int token)
    {
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
        auto& func = state_funcs[State::TOTAL_STATES * current_state + token];
        current_state = state_transitions[State::TOTAL_STATES * current_state + token];
        func();
    }

    // normalize str in place, the resulting size is written in str_len
    void normalize_string_literal(char* str, uint64_t* str_len)
    {
        char* write_ptr = str;
        char* read_ptr = write_ptr + 1; // skip first character: '"'

        *str_len -= 2;
        char* end = str + *str_len + 1;

        UnicodeEscape::normalize_string(read_ptr, write_ptr, end, *str_len);
    }
};
}} // namespace Import::QuadModel
