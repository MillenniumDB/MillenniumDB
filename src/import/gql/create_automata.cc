#include "import.h"

using namespace Import::GQL;

void OnDiskImport::create_automata()
{
    // set all transitions as error at first, transitions that are defined later will stay as error.
    for (int s = 0; s < State::TOTAL_STATES; s++) {
        for (int t = 1; t < Token::TOTAL_TOKENS; t++) {
            set_transition(s, t, State::WRONG_LINE, std::bind(&OnDiskImport::print_error, this));
        }
    }
    // ignore whitespace token
    for (int s = 0; s < State::TOTAL_STATES; s++) {
        set_transition(s, Token::WHITESPACE, s, std::bind(&OnDiskImport::do_nothing, this));
    }

    // wrong line stays wrong (without giving more errors) until endline
    for (int t = 0; t < Token::TOTAL_TOKENS; t++) {
        set_transition(State::WRONG_LINE, t, State::WRONG_LINE, std::bind(&OnDiskImport::do_nothing, this));
    }

    // error clears after an endline
    set_transition(
        State::WRONG_LINE,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
    );

    // ignore empty lines
    set_transition(
        State::LINE_BEGIN,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
    );

    set_transition(
        State::LINE_BEGIN,
        Token::IDENTIFIER,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_identifier, this)
    );

    set_transition(
        State::LINE_BEGIN,
        Token::STRING,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_string, this)
    );

    set_transition(
        State::LINE_BEGIN,
        Token::INTEGER,
        State::FIRST_ID,
        std::bind(&OnDiskImport::save_first_id_int, this)
    );

    set_transition(
        State::FIRST_ID,
        Token::COLON,
        State::EXPECT_NODE_LABEL,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::FIRST_ID,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
    );

    set_transition(
        State::FIRST_ID,
        Token::IDENTIFIER,
        State::EXPECT_NODE_PROP_COLON,
        std::bind(&OnDiskImport::save_prop_key, this)
    );

    set_transition(
        State::FIRST_ID,
        Token::L_ARROW,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::save_direction, this, EdgeDir::LEFT)
    );

    set_transition(
        State::FIRST_ID,
        Token::R_ARROW,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::save_direction, this, EdgeDir::RIGHT)
    );

    set_transition(
        State::FIRST_ID,
        Token::UNDIRECTED,
        State::EXPECT_EDGE_SECOND,
        std::bind(&OnDiskImport::save_direction, this, EdgeDir::UNDIRECTED)
    );

    set_transition(
        State::NODE_DEFINED,
        Token::COLON,
        State::EXPECT_NODE_LABEL,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::NODE_DEFINED,
        Token::IDENTIFIER,
        State::EXPECT_NODE_PROP_COLON,
        std::bind(&OnDiskImport::save_prop_key, this)
    );

    set_transition(
        State::NODE_DEFINED,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
    );

    set_transition(
        State::EXPECT_NODE_LABEL,
        Token::IDENTIFIER,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_label, this)
    );

    set_transition(
        State::EXPECT_NODE_PROP_COLON,
        Token::COLON,
        State::EXPECT_NODE_PROP_VALUE,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::TYPED_STRING,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_datatype, this)
    );

    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::STRING,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_string, this)
    );

    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::INTEGER,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_int, this)
    );

    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::FLOAT,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_float, this)
    );

    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::K_FALSE,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_false, this)
    );

    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::K_TRUE,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::add_node_prop_true, this)
    );

    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::IDENTIFIER,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::save_second_id_identifier, this)
    );

    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::STRING,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::save_second_id_string, this)
    );

    set_transition(
        State::EXPECT_EDGE_SECOND,
        Token::INTEGER,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::save_second_id_int, this)
    );

    set_transition(
        State::EDGE_DEFINED,
        Token::COLON,
        State::EXPECT_EDGE_LABEL,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::EDGE_DEFINED,
        Token::ENDLINE,
        State::LINE_BEGIN,
        std::bind(&OnDiskImport::finish_line, this)
    );

    set_transition(
        State::EDGE_DEFINED,
        Token::IDENTIFIER,
        State::EXPECT_EDGE_PROP_COLON,
        std::bind(&OnDiskImport::save_prop_key, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_COLON,
        Token::COLON,
        State::EXPECT_EDGE_PROP_VALUE,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::EXPECT_EDGE_LABEL,
        Token::IDENTIFIER,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_label, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::TYPED_STRING,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_datatype, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::STRING,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_string, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::INTEGER,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_int, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::FLOAT,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_float, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::K_FALSE,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_false, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::K_TRUE,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::add_edge_prop_true, this)
    );

    // lists
    set_transition(
        State::EXPECT_NODE_PROP_VALUE,
        Token::L_BRACKET,
        State::EXPECT_NODE_LIST_ELEMENT,
        std::bind(&OnDiskImport::init_list, this)
    );

    set_transition(
        State::EXPECT_EDGE_PROP_VALUE,
        Token::L_BRACKET,
        State::EXPECT_EDGE_LIST_ELEMENT,
        std::bind(&OnDiskImport::init_list, this)
    );

    set_transition(
        State::EXPECT_NODE_LIST_ELEMENT,
        Token::K_TRUE,
        State::EXPECT_NODE_LIST_ELEMENT,
        std::bind(&OnDiskImport::add_list_value_true, this)
    );

    set_transition(
        State::EXPECT_NODE_LIST_ELEMENT,
        Token::K_FALSE,
        State::EXPECT_NODE_LIST_ELEMENT,
        std::bind(&OnDiskImport::add_list_value_false, this)
    );

    set_transition(
        State::EXPECT_NODE_LIST_ELEMENT,
        Token::STRING,
        State::EXPECT_NODE_LIST_ELEMENT,
        std::bind(&OnDiskImport::add_list_value_string, this)
    );

    set_transition(
        State::EXPECT_NODE_LIST_ELEMENT,
        Token::INTEGER,
        State::EXPECT_NODE_LIST_ELEMENT,
        std::bind(&OnDiskImport::add_list_value_integer, this)
    );

    set_transition(
        State::EXPECT_EDGE_LIST_ELEMENT,
        Token::K_TRUE,
        State::EXPECT_EDGE_LIST_ELEMENT,
        std::bind(&OnDiskImport::add_list_value_true, this)
    );

    set_transition(
        State::EXPECT_EDGE_LIST_ELEMENT,
        Token::K_FALSE,
        State::EXPECT_EDGE_LIST_ELEMENT,
        std::bind(&OnDiskImport::add_list_value_false, this)
    );

    set_transition(
        State::EXPECT_EDGE_LIST_ELEMENT,
        Token::STRING,
        State::EXPECT_EDGE_LIST_ELEMENT,
        std::bind(&OnDiskImport::add_list_value_string, this)
    );

    set_transition(
        State::EXPECT_EDGE_LIST_ELEMENT,
        Token::INTEGER,
        State::EXPECT_EDGE_LIST_ELEMENT,
        std::bind(&OnDiskImport::add_list_value_integer, this)
    );

    set_transition(
        State::EXPECT_NODE_LIST_ELEMENT,
        Token::L_BRACKET,
        State::EXPECT_NODE_LIST_ELEMENT,
        std::bind(&OnDiskImport::init_list, this)
    );

    set_transition(
        State::EXPECT_EDGE_LIST_ELEMENT,
        Token::L_BRACKET,
        State::EXPECT_EDGE_LIST_ELEMENT,
        std::bind(&OnDiskImport::init_list, this)
    );

    set_transition(
        State::EXPECT_NODE_LIST_ELEMENT,
        Token::COMMA,
        State::EXPECT_NODE_LIST_ELEMENT,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::EXPECT_EDGE_LIST_ELEMENT,
        Token::COMMA,
        State::EXPECT_EDGE_LIST_ELEMENT,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::EXPECT_NODE_LIST_ELEMENT,
        Token::R_BRACKET,
        State::NODE_DEFINED,
        std::bind(&OnDiskImport::save_node_list, this)
    );

    set_transition(
        State::EXPECT_EDGE_LIST_ELEMENT,
        Token::R_BRACKET,
        State::EDGE_DEFINED,
        std::bind(&OnDiskImport::save_edge_list, this)
    );
}
