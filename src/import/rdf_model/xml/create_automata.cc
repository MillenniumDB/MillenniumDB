#include "import.h"

using namespace Import::Rdf::XML;

void OnDiskImport::set_transition(int state, int token, int value, std::function<void()> func)
{
    state_funcs[State::TOTAL_STATES * token + state] = func;
    state_transitions[State::TOTAL_STATES * token + state] = value;
}

int OnDiskImport::get_transition(int state, int token)
{
    state_funcs[State::TOTAL_STATES * token + state]();
    return state_transitions[State::TOTAL_STATES * token + state];
}

void OnDiskImport::create_automata() {
    // Set every transition to error
    for (int state_idx = 0; state_idx < State::TOTAL_STATES; state_idx++) {
        for (int token_idx = 1; token_idx < Token::TOTAL_TOKENS; token_idx++) {
            set_transition(state_idx, token_idx, State::ERROR, std::bind(&OnDiskImport::print_error, this));
        }
    }

    for (int state_idx = 0; state_idx < State::TOTAL_STATES; state_idx++) {
        set_transition(state_idx, Token::ENDLINE, state_idx, std::bind(&OnDiskImport::move_to_next_line, this));
        set_transition(state_idx, Token::WHITESPACE, state_idx, std::bind(&OnDiskImport::do_nothing, this));
        set_transition(state_idx, Token::COMMENT, state_idx, std::bind(&OnDiskImport::do_nothing, this));
    }

    set_transition(
        State::START,
        Token::XML_METADATA,
        State::START,
        std::bind(&OnDiskImport::do_nothing, this)
    );

    set_transition(
        State::START,
        Token::OPENING_TAG,
        State::START,
        std::bind(&OnDiskImport::open_tag, this)
    );

    set_transition(
        State::START,
        Token::TAG_CONTENT,
        State::START,
        std::bind(&OnDiskImport::add_tag_value, this)
    );

    set_transition(
        State::START,
        Token::SELF_CLOSING_TAG,
        State::START,
        std::bind(&OnDiskImport::open_tag, this)
    );

    set_transition(
        State::START,
        Token::CLOSING_TAG,
        State::START,
        std::bind(&OnDiskImport::close_tag, this, false)
    );
}
