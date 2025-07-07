#include "import.h"

using namespace Import::QuadModel::CSV;

void OnDiskImport::create_automata()
{
    // Set every transition to error
    for (int state_idx = 0; state_idx < State::TOTAL_STATES; state_idx++) {
        for (int token_idx = 1; token_idx < Token::TOTAL_TOKENS; token_idx++) {
            set_transition(state_idx, token_idx, State::ERROR, std::bind(&OnDiskImport::print_error, this));
        }
    }

    // Transitions for nodes files
    set_transition(
        State::START_HEADER_NODES,
        Token::UNQUOTED_STRING,
        State::COLUMN_NAME_READ_NODES,
        std::bind(&OnDiskImport::save_header_column, this)
    );

    set_transition(
        State::START_HEADER_NODES,
        Token::STRING,
        State::COLUMN_NAME_READ_NODES,
        std::bind(&OnDiskImport::save_header_column, this)
    );

    set_transition(
        State::START_HEADER_NODES,
        Token::INTEGER,
        State::COLUMN_NAME_READ_NODES,
        std::bind(&OnDiskImport::save_header_column, this)
    );

    set_transition(
        State::START_HEADER_NODES,
        Token::FLOAT,
        State::COLUMN_NAME_READ_NODES,
        std::bind(&OnDiskImport::save_header_column, this)
    );

    // Save column into stack vector
    set_transition(
        State::COLUMN_NAME_READ_NODES,
        Token::SEP_COLUMN,
        State::START_HEADER_NODES,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::COLUMN_NAME_READ_NODES,
        Token::ENDLINE,
        State::START_BODY_NODES,
        std::bind(&OnDiskImport::verify_anon, this)
    );

    // Starting to read the column and we can't determine the type of data.
    set_transition(
        State::START_BODY_NODES,
        Token::UNQUOTED_STRING,
        State::COLUMN_BODY_NODES_READ,
        std::bind(&OnDiskImport::save_body_column_to_buffer, this)
    );

    // Starting to read the column and we determine is a string
    set_transition(
        State::START_BODY_NODES,
        Token::STRING,
        State::COLUMN_BODY_NODES_READ,
        std::bind(&OnDiskImport::save_body_column_to_buffer, this)
    );

    // Starting to read the column and we determine is an integer
    set_transition(
        State::START_BODY_NODES,
        Token::INTEGER,
        State::COLUMN_BODY_NODES_READ,
        std::bind(&OnDiskImport::save_body_column_to_buffer, this)
    );

    // Starting to read the column and we determine is a float
    set_transition(
        State::START_BODY_NODES,
        Token::FLOAT,
        State::COLUMN_BODY_NODES_READ,
        std::bind(&OnDiskImport::save_body_column_to_buffer, this)
    );

    set_transition(
        State::COLUMN_BODY_NODES_READ,
        Token::SEP_COLUMN,
        State::START_BODY_NODES,
        std::bind(&OnDiskImport::move_to_next_column, this)
    );

    set_transition(
        State::START_BODY_NODES,
        Token::SEP_COLUMN,
        State::START_BODY_NODES,
        std::bind(&OnDiskImport::save_empty_body_column_to_buffer, this)
    );

    set_transition(
        State::COLUMN_BODY_NODES_READ,
        Token::ENDLINE,
        State::START_BODY_NODES,
        std::bind(&OnDiskImport::process_node_line, this)
    );

    // Transitions for edges files
    set_transition(
        State::START_HEADER_EDGES,
        Token::UNQUOTED_STRING,
        State::COLUMN_NAME_READ_EDGES,
        std::bind(&OnDiskImport::save_header_column, this)
    );

    set_transition(
        State::START_HEADER_EDGES,
        Token::STRING,
        State::COLUMN_NAME_READ_EDGES,
        std::bind(&OnDiskImport::save_header_column, this)
    );

    set_transition(
        State::START_HEADER_EDGES,
        Token::INTEGER,
        State::COLUMN_NAME_READ_EDGES,
        std::bind(&OnDiskImport::save_header_column, this)
    );

    set_transition(
        State::START_HEADER_EDGES,
        Token::FLOAT,
        State::COLUMN_NAME_READ_EDGES,
        std::bind(&OnDiskImport::save_header_column, this)
    );

    set_transition(
        State::COLUMN_NAME_READ_EDGES,
        Token::SEP_COLUMN,
        State::START_HEADER_EDGES,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::COLUMN_NAME_READ_EDGES,
        Token::ENDLINE,
        State::START_BODY_EDGES,
        std::bind(&OnDiskImport::verify_edge_file_header, this)
    );

    set_transition(
        State::START_BODY_EDGES,
        Token::UNQUOTED_STRING,
        State::COLUMN_BODY_EDGES_READ,
        std::bind(&OnDiskImport::save_body_column_to_buffer, this)
    );

    set_transition(
        State::START_BODY_EDGES,
        Token::STRING,
        State::COLUMN_BODY_EDGES_READ,
        std::bind(&OnDiskImport::save_body_column_to_buffer, this)
    );

    set_transition(
        State::START_BODY_EDGES,
        Token::INTEGER,
        State::COLUMN_BODY_EDGES_READ,
        std::bind(&OnDiskImport::save_body_column_to_buffer, this)
    );

    set_transition(
        State::START_BODY_EDGES,
        Token::FLOAT,
        State::COLUMN_BODY_EDGES_READ,
        std::bind(&OnDiskImport::save_body_column_to_buffer, this)
    );

    set_transition(
        State::COLUMN_BODY_EDGES_READ,
        Token::SEP_COLUMN,
        State::START_BODY_EDGES,
        std::bind(&OnDiskImport::move_to_next_column, this)
    );

    set_transition(
        State::START_BODY_EDGES,
        Token::SEP_COLUMN,
        State::START_BODY_EDGES,
        std::bind(&OnDiskImport::save_empty_body_column_to_buffer, this)
    );

    set_transition(
        State::COLUMN_BODY_EDGES_READ,
        Token::ENDLINE,
        State::START_BODY_EDGES,
        std::bind(&OnDiskImport::save_edge_line, this)
    );

    set_transition(
        State::COLUMN_BODY_NODES_READ,
        Token::END_OF_FILE,
        State::START_BODY_NODES,
        std::bind(&OnDiskImport::process_node_line, this)
    );

    set_transition(
        State::COLUMN_BODY_EDGES_READ,
        Token::END_OF_FILE,
        State::START_BODY_EDGES,
        std::bind(&OnDiskImport::save_edge_line, this)
    );

    set_transition(
        State::START_BODY_NODES,
        Token::END_OF_FILE,
        State::START_BODY_NODES,
        std::bind(&OnDiskImport::process_node_line, this)
    );

    set_transition(
        State::START_BODY_EDGES,
        Token::END_OF_FILE,
        State::START_BODY_EDGES,
        std::bind(&OnDiskImport::save_edge_line, this)
    );
}
