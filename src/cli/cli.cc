#ifndef NO_MDB_CLI
#include "cli.h"

#include <codecvt>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "misc/logger.h"
#include "query/optimizer/property_graph_model/executor_constructor.h"
#include "query/optimizer/quad_model/executor_constructor.h"
#include "query/optimizer/rdf_model/executor_constructor.h"
#include "query/parser/gql_query_parser.h"
#include "query/parser/mql_query_parser.h"
#include "query/parser/sparql_query_parser.h"

#define _XOPEN_SOURCE 700
#ifdef __APPLE__
#include <curses.h>
#elif __linux
#include <cursesw.h>
#endif
// #include <term.h>

// Undefine curses macros that collide with identifiers
#undef timeout

// Useful commands:
// man ncurses
// man terminfo
// man curs_terminfo
// infocmp -a
// showkey -a

namespace chrono = std::chrono;

namespace CLI {

std::wstring_convert<std::codecvt_utf8<wchar_t>> string_converter;


static std::wstring string_to_wstring(const std::string& string) {
    return string_converter.from_bytes(string);
}


static std::string wstring_to_string(const std::wstring& string) {
    return string_converter.to_bytes(string);
}


// Used to save the terminal name to handle a special case for xterm
std::string term_name;

// ncurses key codes for terminfo extensions
wint_t KEY_CODE_SHIFT_UP   = 0;
wint_t KEY_CODE_SHIFT_DOWN = 0;
wint_t KEY_CODE_CTRL_UP    = 0;
wint_t KEY_CODE_CTRL_DOWN  = 0;
wint_t KEY_CODE_CTRL_LEFT  = 0;
wint_t KEY_CODE_CTRL_RIGHT = 0;
wint_t KEY_CODE_CTRL_HOME  = 0;
wint_t KEY_CODE_CTRL_END   = 0;

// ASCII control characters
constexpr wint_t CHAR_CTRL_S = 0x13;
constexpr wint_t CHAR_CTRL_N = 0x0e;
constexpr wint_t CHAR_CTRL_P = 0x10;
constexpr wint_t CHAR_CTRL_C = 0x03;
constexpr wint_t CHAR_CTRL_D = 0x04;
constexpr wint_t CHAR_CTRL_O = 0x0f;
constexpr wint_t CHAR_CTRL_H = 0x08;
constexpr wint_t CHAR_CTRL_L = 0x0C;
constexpr wint_t CHAR_CTRL_U = 0x15;
constexpr wint_t CHAR_CTRL_K = 0x0B;
constexpr wint_t CHAR_DEL    = 0x7F;
constexpr wint_t CHAR_TAB    = 0x09;
constexpr wint_t CHAR_ENTER  = 0x0A;

// Color pairs
constexpr short CP_NORMAL  = 1;
constexpr short CP_PRIMARY = 2;
constexpr short CP_OK      = 3;
constexpr short CP_ERROR   = 4;

// utf-8 prefixes and masks
constexpr wchar_t UTF_1_PREFIX      = 0b0000'0000;
constexpr wchar_t UTF_1_PREFIX_MASK = 0b1000'0000;

constexpr wchar_t UTF_2_PREFIX      = 0b1100'0000;
constexpr wchar_t UTF_2_PREFIX_MASK = 0b1110'0000;

constexpr wchar_t UTF_3_PREFIX      = 0b1110'0000;
constexpr wchar_t UTF_3_PREFIX_MASK = 0b1111'0000;

constexpr wchar_t UTF_4_PREFIX      = 0b1111'0000;
constexpr wchar_t UTF_4_PREFIX_MASK = 0b1111'1000;

constexpr wchar_t UTF_CONT_PREFIX      = 0b1000'0000;
constexpr wchar_t UTF_CONT_PREFIX_MASK = 0b1100'0000;

// border drawing characters
const std::string hline     = wstring_to_string(L"─");
const std::string vline     = wstring_to_string(L"│");
const std::string top_left  = wstring_to_string(L"┌");
const std::string top_mid   = wstring_to_string(L"┬");
const std::string top_right = wstring_to_string(L"┐");
const std::string mid_left  = wstring_to_string(L"├");
const std::string mid_mid   = wstring_to_string(L"┼");
const std::string mid_right = wstring_to_string(L"┤");
const std::string bot_left  = wstring_to_string(L"└");
const std::string bot_mid   = wstring_to_string(L"┴");
const std::string bot_right = wstring_to_string(L"┘");


constexpr std::array<wchar_t, 4> process_spinner {L'|', L'/', L'-', L'\\'};


// fix for mac so we do not have to use nullptr
// and do not have to define a wint_t everywhere.
wint_t throw_away_input;

// Uses terminfo to get key-codes for a terminal capability
static int keycode_from_termcap(const char* termcap) {
    const char* escape_sequence = tigetstr(termcap);

    if (escape_sequence == reinterpret_cast<char*>(-1)) {
        // termcap is not a string capability
        return 0;
    }
    if (escape_sequence == 0) {
        // termcap is canceled or absent from the terminal description
        return 0;
    }

    int keycode = key_defined(escape_sequence);

    if (keycode == 0) {
        // No keycode is bound to escape sequence
        return 0;
    }
    if (keycode == -1) {
        // escape sequence conflicts with longer strings which are bound to keys
        return 0;
    }

    return keycode;
}


enum class LineType : uint8_t {
    BlankLine,   // Blank line
    InputHeader, // First line of the input (starts with prompt)
    InputCont,   // Input continuation lines (starts with space)
    OutputOk,    // Normal output line
    OutputError  // Error output line
};


// Represents a line of output
class Line {
public:
    const std::wstring data;
    const LineType type = LineType::OutputOk;

    Line(std::wstring data, LineType type) : data(std::move(data)), type(type) {};

    int size() {
        return static_cast<int>(data.size());
    }
};


// Creates a table divider line
static std::string make_divider(const std::vector<int>& widths,
                                int                     indentation,
                                const std::string&      hline,
                                const std::string&      start,
                                const std::string&      middle,
                                const std::string&      end) {
    std::string divider;

    if (widths.size() >= 1) {
        divider += std::string(indentation, ' ');
        divider += start;
    }

    for (size_t i = 0; i < widths.size(); i++) {
        for (int j = 0; j < widths[i] + 2; j++) {
            divider += hline;
        }

        if (i < widths.size() - 1) {
            divider += middle;
        }
    }

    if (widths.size() >= 1) {
        divider += end;
    }

    divider += '\n';
    return divider;
}


// Used to buffer writes to an ostream
class BufferedWriter {
public:
    BufferedWriter(std::ostream& stream) : stream(stream) { }
    ~BufferedWriter() {
        FlushBuffer();
        delete[] buffer;
    }

    BufferedWriter(BufferedWriter&) = delete;

    void put(char ch) {
        if (count >= buffer_size) {
            FlushBuffer();
        }
        buffer[count] = ch;
        count++;
    }

    void write(const char* arr, size_t size) {
        if (size > buffer_size) {
            fail = true;
            return;
        }

        if (size > buffer_size - count) {
            FlushBuffer();
        }

        std::memcpy(buffer+count, arr, size);
        count += size;
    }

    bool fail = false;

private:
    static constexpr int buffer_size = 10 * 1024 * 1024;
    char* buffer = new char[buffer_size];

    size_t count = 0;
    std::ostream& stream;

    void FlushBuffer() {
        stream.write(buffer, count);
        count = 0;
        if(stream.fail()) {
            fail = true;
        }
    }
};
static_assert(!std::is_copy_assignable_v<BufferedWriter>);
static_assert(!std::is_copy_constructible_v<BufferedWriter>);
static_assert(!std::is_move_constructible_v<BufferedWriter>);
static_assert(!std::is_move_constructible_v<BufferedWriter>);


// Used to buffer reads from an istream
class BufferedReader {
public:
    BufferedReader(std::istream& stream) : stream(stream) { }
    ~BufferedReader() {
        delete[] buffer;
    }

    BufferedReader(BufferedReader&) = delete;

    char get() {
        if (next >= count) {
            if (reached_stream_end) {
                end = true;
                return 0;
            } else {
                FillBuffer();
            }
        }

        auto ch = buffer[next];
        next++;
        return ch;
    }

    bool has_more() {
        if (next < count) {
            return true;
        } else {
            if (reached_stream_end) {
                end = true;
                return false;
            } else {
                FillBuffer();
                return has_more();
            }
        }
    }

    bool end = false;

private:
    static constexpr int buffer_size = 10 * 1024 * 1024;
    char* buffer = new char[buffer_size];

    size_t count = 0;
    size_t next = 0;
    std::istream& stream;

    bool reached_stream_end = false;


    void FillBuffer() {
        stream.read(buffer, buffer_size);
        count = stream.gcount();
        next = 0;
        if(stream.fail()) {
            reached_stream_end = true;
        }

    }
};
static_assert(!std::is_copy_assignable_v<BufferedReader>);
static_assert(!std::is_copy_constructible_v<BufferedReader>);
static_assert(!std::is_move_constructible_v<BufferedReader>);
static_assert(!std::is_move_constructible_v<BufferedReader>);

enum class ReturnStatus { Ok, Error, Interrupted };

// Main class that implements the CLI
class CLI {
public:
    CLI(Model model, chrono::seconds timeout) : model(model), timeout(timeout) { }
    ~CLI();

    // Initialize Tui, has to be called before MainLoop
    int Init();
    // Main loop that processes user input
    int MainLoop();

    // Show help in output window
    void ShowHelp();

    // Try to read history from file
    void ReadHistoryFile();

    CLI(const CLI&)            = delete;
    CLI& operator=(const CLI&) = delete;

private:
    // Try to append history entry to history file
    void AppendHistoryEntryToFile(const std::vector<std::wstring>& history_entry);

    // Handle moving though history repeatedly using arrow keys
    void UpdateHistory();
    // Update ncurses window positions, sizes and clear
    void UpdateWindows();
    // Update clamp cursors and window offsets
    void UpdateOffsets();
    // Update windows and write to screen
    void UpdateScreen();
    // Do a complete update (calls the previous update functions)
    void Update();

    // Inserts newline
    void HandleKeyEnter();
    // Delete character to the left of cursor
    void HandleKeyBackspace();
    // Delete character under the cursor
    void HandleKeyDelete();
    // Move one line up or move back through input history
    void HandleKeyUp();
    // Move one line down or move forward through input history
    void HandleKeyDown();
    // Move cursor to the left
    void HandleKeyLeft();
    // Move cursor to the right
    void HandleKeyRight();
    // Handle indentation using tab
    void HandleKeyTab();

    // Backspace an entire word
    void HandleBackspaceWord();
    // Move entire word to left
    void HandleWordLeft();
    // Move entire word to right
    void HandleWordRight();
    // Join two lines
    void HandleJoinLines();
    // Delete a whole line
    void HandleDeleteLine();

    // Move backwards in the input history
    void HandlePrevHistory();
    // Move forwards in the input history
    void HandleNextHistory();

    // Execute the input
    void HandleSend();
    // Handle scrolling using mouse
    void HandleMouse();
    // Handle input of normal characters
    void HandleNormalChars(const wchar_t ch);

    // Show pager with last output
    void ShowPager();

    void ProcessSPARQLQuery(std::ostream& os, const std::string& query);
    void ProcessMQLQuery(std::ostream& os, const std::string& query);
    void ProcessGQLQuery(std::ostream& os, const std::string& query);

    // Updates a spinner to indicate progress
    void update_process_spinner();

    // Calculates column width of text in TSV format
    ReturnStatus calculate_column_widths(std::istream& input_stream, std::vector<int>& widths);
    // Transforms input_stream into a table and writes to output_stream
    ReturnStatus tabulate(std::istream& input_stream, std::ostream& output_stream);

    // Adds a line to the output window
    void AddOutput(std::wstring, LineType);

    // Prints a Line to the a window
    void PrintToWindow(const Line& line, WINDOW* win, int y, int offset);

    // Differentiates between the different models
    Model model;

    // Used for thread that executes queries
    std::atomic<bool> query_is_done = false;
    ReturnStatus query_return_status;
    int query_result_count = -1;
    chrono::milliseconds query_execution_time;

    // Query context used by thread executing a query
    QueryContext qc;
    // Used to advance and updates process spinner character
    int process_spinner_idx = 0;

    // Show output of queries, messages, etc
    WINDOW* output_win;
    // Divider between output and input windows, shows the buttons
    WINDOW* divider_win;
    // The input windows where the user's input is shown
    WINDOW* input_win;
    // A windows the the left of the input window to show the input prompt
    WINDOW* prompt_win;

    // Holds the user's input, also used to show progress during query execution
    std::vector<std::wstring>              input_data = { L"" };
    // Holds the user's input history
    std::vector<std::vector<std::wstring>> input_history;
    // Holds the content shown in the output window, such as query results and messages
    std::vector<Line>                      output_lines;

    // File-stream of the history file.
    std::wfstream history_fs;

    // The path of the file holding the output of the last executed query
    std::optional<std::filesystem::path> last_output_tmp_file;

    // Indicates that the user is currently moving through history.
    // This is used to differentiate between moving through history
    // and moving the cursor up and down, using arrow keys
    bool moving_through_history = false;
    // Indicates that the previous input moved through history.
    bool moved_through_history  = false;
    // Hols the index of the current history entry (last history entry if not moving through history).
    int  current_history_entry  = 0;

    // Input cursor y and x position
    int cursor_y = 0;
    int cursor_x = 0;

    // Input y and x offset to handle scrolling input window
    int input_offset_y = 0;
    int input_offset_x = 0;

    // Output y and x offset to handle scrolling output window
    int output_offset_y = 0;
    int output_offset_x = 0;

    // Width of automatic indentation
    int indentation     = 4;

    // Maximum number of query output lines to show in output window
    int max_table_lines = 14;

    // The width of the biggest line in the output
    int output_max_width = 0;

    // Timeout of query execution in seconds
    chrono::seconds timeout {60};

    // Prompt shown in prompt window and output of queries
    std::wstring prompt_start = L"> ";
    // Continuation prompt (only shown in input window, not output window)
    std::wstring prompt_cont  = L". ";

    // "Buttons" shown in the divider window
    std::vector<std::wstring> buttons = {
        L" F1 Help ",
        L" Ctrl+S Send ",
        L" Ctrl+D Exit ",
    };

    // Prompt used to show less
    std::string pager_cmd =
      "LESSCHARSET=utf-8"       // Use utf-8 for less
      " less -+--LONG-PROMPT"   // Do short prompt, to avoid having to calculate line numbers on start-up
      " -+--long-prompt"        // Do short prompt, to avoid having to calculate line numbers on start-up
      " -+--quit-if-one-screen" // Do not quit if the output fits on one screen
      " --chop-long-lines"      // Do not wrap line lines
      " --ignore-case"          // Only make search case sensitive when pattern contains at leas one capital letter
      " --quit-on-intr"         // Quit with Ctrl+C
      " --clear-screen"         // Clear whole screen on start-up
      " ";                      // Space between last option and file argument

    // CLI help text
    const std::vector<std::wstring> help_text = {
        { L"            Welcome to MillenniumDB"},
        { L""},
        { L" Global:"},
        { L"     F1                : Show this help"},
        { L"     Ctrl+D            : Exit"},
        { L""},
        { L" Pager (Less):"},
        { L"     Ctrl+L            : Show last output in pager"},
        { L"     Q                 : Exit the pager"},
        { L"     H                 : Show pager help"},
        { L""},
        { L" Input window:"},
        { L"     Ctrl+S            : Send input"},
        { L"     Ctrl+C            : Clear input"},
        { L"     Arrows            : Move cursor"},
        { L"     Up/Down           : Go through history if input is empty"},
        { L"     Ctrl+P/N          : Go through history"},
        { L"     Ctrl+Backspace    : Delete word"},
        { L"     Ctrl+U            : Join (unite) lines"},
        { L"     Ctrl+K            : Delete (kill) line"},
        { L"     Home/End          : Move cursor to start/end of line"},
        { L"     Ctrl+Home/End     : Move cursor to start/end of input"},
        { L"     Ctrl+Left/Right   : Move cursor to previous/next word"},
        { L"     Mouse Wheel       : Scroll up/down"},
        { L"     Shift+Mouse wheel : Scroll left/right"},
        { L""},
        { L" Output window:"},
        { L"     Ctrl+O            : Clear output"},
        { L"     Shift+Arrows      : Scroll output"},
        { L"     PageUp/PageDown   : Scroll output up/down one page"},
        { L"     Mouse Wheel       : Scroll up/down"},
        { L"     Shift+Mouse wheel : Scroll left/right"},
    };
};
static_assert(!std::is_copy_assignable_v<CLI>);
static_assert(!std::is_copy_constructible_v<CLI>);
static_assert(!std::is_move_assignable_v<CLI>);
static_assert(!std::is_move_constructible_v<CLI>);


void CLI::ReadHistoryFile() {
    auto home_var = std::getenv("HOME");
    if (home_var == nullptr) {
        AddOutput(
            L" HOME environment variable not set, not loading command history",
            LineType::OutputError);
        return;
    }

    auto history_file_path = std::filesystem::path(home_var) / ".millenniumdb" / "history";
    if (!std::filesystem::exists(history_file_path)) {
        try {
            std::filesystem::create_directories(history_file_path.parent_path());
        } catch (...) {
            AddOutput(L" Could not create " + history_file_path.parent_path().wstring()
                + L", not loading command history", LineType::OutputError);
            return;
        }
    } else if (!std::filesystem::is_regular_file(history_file_path)) {
        AddOutput(L' ' + history_file_path.wstring()
            + L"is not a file, not loading command history", LineType::OutputError);
        return;
    }

    history_fs.open(history_file_path, std::ios_base::app | std::ios_base::out);
    history_fs.close();
    history_fs.open(history_file_path, std::ios_base::in);

    if (history_fs.fail()) {
        AddOutput(L" Could not open " + history_file_path.wstring()
            + L" in RW mode, not loading command history", LineType::OutputError);
        history_fs.close();
        return;
    }

    input_history.clear();

    constexpr size_t buffer_size = 1024*1024;
    wchar_t buffer[buffer_size];
    while (history_fs.getline(buffer, buffer_size)) {
        if (history_fs.gcount() < 1) {
            continue;
        }
        if (buffer[0] == '>') {
            input_history.push_back({std::wstring(&buffer[1])});
        } else if (buffer[0] == ' ') {
            input_history.back().emplace_back(&buffer[1]);
        }
    }

    history_fs.close();
    history_fs.open(history_file_path, std::ios_base::app | std::ios_base::out);
}


void CLI::AppendHistoryEntryToFile(const std::vector<std::wstring>& history_entry) {
    if (!history_fs.is_open()) {
        return;
    }
    history_fs.clear();

    auto first = true;
    for (auto& line : history_entry) {
        if (first) {
            first = false;
            history_fs.put(L'>');
        } else {
            history_fs.put(L' ');
        }
        history_fs.write(line.c_str(), line.size());
        history_fs.write(L"\n", 1);

        if (history_fs.fail()) {
            AddOutput(L"", LineType::BlankLine);
            AddOutput(L"Error while writing to history file", LineType::OutputError);
        }
    }

    history_fs.flush();
}


int CLI::Init() {
    for (auto& cat : logger.categories) {
        cat.second.enabled = false;
    }

    setlocale(LC_ALL, "en_US.UTF-8");
    std::locale::global(std::locale("en_US.UTF-8"));

    if (initscr() == nullptr) {
        std::cerr << "Curses initscr() failed";
        return EXIT_FAILURE;
    }
    if (start_color() == ERR) {
        std::cerr << "Curses start_color() failed";
        return EXIT_FAILURE;
    }
    if (use_default_colors() == ERR) {
        std::cerr << "Curses use_default_colors() failed";
        return EXIT_FAILURE;
    }
    if (raw() == ERR) {
        std::cerr << "Curses raw() failed";
        return EXIT_FAILURE;
    }
    if (noecho() == ERR) {
        std::cerr << "Curses noecho() failed";
        return EXIT_FAILURE;
    }
    if (intrflush(stdscr, FALSE) == ERR) {
        std::cerr << "Curses intrflush(stdscr, FALSE) failed";
        return EXIT_FAILURE;
    }
    if (keypad(stdscr, TRUE) == ERR) {
        std::cerr << "Curses keypad(stdscr) failed";
        return EXIT_FAILURE;
    }
    if (COLOR_PAIRS < 4) {
        std::cerr << "Curses not enough color pairs";
        return EXIT_FAILURE;
    }

    // Initialize color pairs
    init_pair(CP_NORMAL, -1,          -1);
    init_pair(CP_PRIMARY, COLOR_CYAN,  -1);
    init_pair(CP_OK,     COLOR_GREEN, -1);
    init_pair(CP_ERROR,  COLOR_RED,   -1);

    // Set initial color pair
    attrset(COLOR_PAIR(CP_NORMAL));

    // Create ncurses windows
    output_win  = newwin(LINES - 2, COLS,                       0,         0);
    divider_win = newwin(1,         COLS,                       LINES - 2, 0);
    input_win   = newwin(1,         COLS - prompt_start.size(), LINES - 1, prompt_start.size());
    prompt_win  = newwin(1,         prompt_start.size(),        LINES - 1, 0);

    // Enable automatic translation of escape-sequences to key-codes
    if (keypad(input_win, TRUE) == ERR) {
        std::cerr << "Curses keypad(input_win, TRUE) failed";
        return EXIT_FAILURE;
    }

    // Get the name of the terminal
    term_name = termname();

    // Initialize key-codes using terminfo capabilities
    // These capabilities are xterm extensions supported by most terminals
    KEY_CODE_SHIFT_UP = keycode_from_termcap("kUP");
    KEY_CODE_SHIFT_DOWN = keycode_from_termcap("kDN");
    KEY_CODE_CTRL_UP = keycode_from_termcap("kUP5");
    KEY_CODE_CTRL_DOWN = keycode_from_termcap("kDN5");
    KEY_CODE_CTRL_LEFT = keycode_from_termcap("kLFT5");
    KEY_CODE_CTRL_RIGHT = keycode_from_termcap("kRIT5");
    KEY_CODE_CTRL_HOME = keycode_from_termcap("kHOM5");
    KEY_CODE_CTRL_END = keycode_from_termcap("kEND5");

    // Enable scroll wheel events
    mousemask(BUTTON4_PRESSED | BUTTON5_PRESSED, nullptr);

    return EXIT_SUCCESS;
}


CLI::~CLI() {
    endwin();
}


void CLI::AddOutput(std::wstring str, LineType type) {
    output_max_width = std::max(output_max_width, static_cast<int>(str.size()));
    output_lines.emplace_back(std::move(str), type);
    output_offset_y = 0;
    output_offset_x = 0;
}



 void CLI::PrintToWindow(const Line& line, WINDOW* win, int y, int offset) {
    const wchar_t* line_cstr = line.data.c_str();

    switch (line.type) {
        case LineType::BlankLine: {
            break;
        }
        case LineType::InputHeader:
        case LineType::InputCont: {
            int prompt_width = prompt_start.size();

            const wchar_t* prompt_cstr;
            auto spaces = std::wstring(prompt_width, ' ');
            if (line.type == LineType::InputHeader) {
                prompt_cstr = prompt_start.c_str();
            } else {
                prompt_cstr = spaces.c_str();
            }

            if (offset < prompt_width) {
                wattrset(win, COLOR_PAIR(CP_PRIMARY) | WA_BOLD);
                mvwaddnwstr(win, y, 0, prompt_cstr + offset, prompt_width-offset);
                wattrset(win, COLOR_PAIR(CP_NORMAL));
                if (offset - prompt_width + 1 < static_cast<int>(line.data.size())) {
                    mvwaddnwstr(win, y, prompt_width-offset, line_cstr, getmaxx(win) + 1 - (prompt_width-offset));
                }
            } else {
                mvwaddnwstr(win, y, 0, line_cstr + offset - prompt_width, getmaxy(output_win) + 1);
            }
            break;
        }
        case LineType::OutputError: {
            wattrset(win, COLOR_PAIR(CP_ERROR));
            mvwaddnwstr(win, y, 0, line_cstr + offset, getmaxx(win) + 1);
            wattrset(win, COLOR_PAIR(CP_NORMAL));
            break;
        }
        default: {
            mvwaddnwstr(win, y, 0, line_cstr + offset, getmaxx(win) + 1);
            break;
        }
    }
}


void CLI::ShowPager() {
    if (!last_output_tmp_file.has_value()) {
        return;
    }

    auto cmd = pager_cmd + last_output_tmp_file->string();
    endwin();

    auto status = std::system(cmd.c_str());
    // When exiting less using Ctrl+C status code 2 or 512 is returned, depending on system
    if (status != 0 && status != 2 && status != 512) {
        AddOutput(L" less not available, not showing output in pager", LineType::OutputError);
    }

    Init();
    Update();
}


void CLI::ShowHelp() {
    AddOutput(L"", LineType::BlankLine);
    for (auto& line : help_text) {
        AddOutput(line, LineType::OutputOk);
    }
}


void CLI::HandleKeyBackspace() {
    if (cursor_x > 0) {
        auto& line = input_data[cursor_y];

        auto count = std::min({indentation, cursor_x ,static_cast<int>(line.size())});
        for (int i = 0; i < cursor_x; i++) {
            if (line[i] != ' ') {
                count = 1;
            }
        }

        line.erase(cursor_x - count, count);
        cursor_x -= count;

    } else if (cursor_x == 0 && cursor_y > 0) {
        cursor_x = input_data[cursor_y-1].size();
        input_data[cursor_y-1] += std::move(input_data[cursor_y]);
        input_data.erase(input_data.begin() + cursor_y);
        cursor_y--;
    }
}


void CLI::HandleKeyDelete() {
    if (cursor_x < static_cast<int>(input_data[cursor_y].size())) {
        auto& line = input_data[cursor_y];
        line.erase(cursor_x, 1);
    } else if (cursor_x == static_cast<int>(input_data[cursor_y].size())
               && cursor_y + 1 < static_cast<int>(input_data.size()))
    {
        input_data[cursor_y] += std::move(input_data[cursor_y + 1]);
        input_data.erase(input_data.begin() + cursor_y + 1);
    }
}


void CLI::HandleKeyUp() {
    if (moving_through_history || (input_data.size() == 1 && input_data[0].empty())) {
        HandlePrevHistory();
    } else  {
        cursor_y--;
    }
}


void CLI::HandleKeyDown() {
    if (moving_through_history || (input_data.size() == 1 && input_data[0].empty())) {
        HandleNextHistory();
    } else {
        cursor_y++;
    }
}


void CLI::HandleKeyLeft() {
    if (cursor_x > 0) {
        cursor_x--;
    } else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = input_data[cursor_y].size();
    }
}


void CLI::HandleKeyRight() {
    if (cursor_x < static_cast<int>(input_data[cursor_y].size())) {
        cursor_x++;
    } else if (cursor_y + 1 < static_cast<int>(input_data.size())) {
        cursor_y++;
        cursor_x = 0;
    }
}


void CLI::HandleKeyEnter() {
    auto& line = input_data[cursor_y];

    // if (cursor_y == static_cast<int>(input_data.size()) - 1
    //     && cursor_x == static_cast<int>(line.size())
    //     && line.back() == ';')
    // {
    //     HandleSend();
    //     return;
    // }

    auto spaces = line.find_first_not_of(' ');
    if (spaces == std::wstring::npos) {
        spaces = line.size();
    }

    auto line_end = line.substr(cursor_x);
    line.resize(cursor_x);
    line_end = std::wstring(spaces, ' ') + line_end;

    input_data.insert(input_data.begin() + cursor_y + 1, std::move(line_end));
    cursor_y++;
    cursor_x = spaces;
}


void CLI::HandleSend() {
    int start = 0;
    int end = input_data.size() - 1;

    while (start < static_cast<int>(input_data.size())
           && input_data[start].find_first_not_of(' ') == std::wstring::npos)
    {
        start++;
    }

    while (end > 0 && input_data[end].find_first_not_of(' ') == std::wstring::npos) {
        end--;
    }

    if (start > end) {
        AddOutput(L"", LineType::BlankLine);
        AddOutput(L" Empty query", LineType::OutputError);
        input_data = {L""};
        return;
    }

    std::string query;

    auto first = true;
    for (int i = start; i <= end; i++) {
        auto& line = input_data[i];
        if (first) {
            first = false;
            AddOutput(L"", LineType::BlankLine);
            AddOutput(line, LineType::InputHeader);
        } else {
            AddOutput(line, LineType::InputCont);
        }
        query.append(wstring_to_string(line));
        query.append("\n");
    }

    // if (query.size() >= 2 && query[query.size()-2] == ';') {
    //     query.resize(query.size() - 1);
    //     query.back() = '\n';
    // }

    if (input_history.empty() || input_data != input_history.back()) {
        AppendHistoryEntryToFile(input_data);
        input_history.push_back(input_data);
    }
    current_history_entry = input_history.size();

    auto path_raw   = std::filesystem::temp_directory_path() / ("mdb-cli.tsv");
    auto path_table = std::filesystem::temp_directory_path() / ("mdb-cli.txt");

    auto fs_raw = std::fstream(path_raw, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
    auto fs_table = std::fstream(path_table, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);


    if (fs_raw.fail()) {
        AddOutput(L" Could not open " + path_raw.wstring() + L" in mode RW", LineType::OutputError);
        input_data = {L""};
        return;
    }

    if (fs_table.fail()) {
        AddOutput(L" Could not open " + path_table.wstring() + L" in mode RW", LineType::OutputError);
        input_data = {L""};
        return;
    }

    input_data = {L"Executing query...  "};
    query_result_count = -1;
    query_is_done = false;
    std::thread query_execution_thread;

    if (model == Model::RDF) {
        query_execution_thread = std::thread(&CLI::ProcessSPARQLQuery, this, std::ref(fs_raw), std::move(query));
    } else if (model == Model::Quad) {
        query_execution_thread = std::thread(&CLI::ProcessMQLQuery, this, std::ref(fs_raw), std::move(query));
    } else {
        query_execution_thread = std::thread(&CLI::ProcessGQLQuery, this, std::ref(fs_raw), std::move(query));
    }

    // Turn off blocking input
    nodelay(input_win, true);
    first = true;
    auto user_interrupted = false;
    while (true) {
        // Check if query execution has finished
        if (query_is_done) {
            query_execution_thread.join();
            break;
        }
        // Handle timeout
        if (qc.thread_info.timeout <= chrono::system_clock::now()) {
            qc.thread_info.interruption_requested = true;
        }
        // Check for user timeout request
        wint_t input;
        int input_type = wget_wch(input_win, &input);
        if (input_type == OK && input == CHAR_CTRL_C) {
            user_interrupted = true;
            qc.thread_info.interruption_requested = true;
        }

        if (first) {
            first = false;
        } else {
            update_process_spinner();
        }
        std::this_thread::sleep_for(chrono::milliseconds(100));
    }

    while (wget_wch(input_win, &throw_away_input) != ERR);

    if (query_return_status == ReturnStatus::Error) {
        input_data = {L""};
        while (wget_wch(input_win, &throw_away_input) != ERR);
        nodelay(input_win, false);
        return;
    } else if (query_return_status == ReturnStatus::Interrupted) {
        AddOutput(L"", LineType::BlankLine);
        if (user_interrupted) {
            AddOutput(L" User interrupted query", LineType::OutputError);
        } else {
            AddOutput(L" Timeout after " + std::to_wstring(timeout.count()) + L" seconds", LineType::OutputError);
        }
    }

    fs_raw.clear();
    fs_raw.seekg(0, std::ios_base::beg);
    if (fs_raw.fail()) {
        AddOutput(L"", LineType::BlankLine);
        AddOutput(L" Could not seek " + path_raw.wstring(), LineType::OutputError);
        input_data = {L""};
        while (wget_wch(input_win, &throw_away_input) != ERR);
        nodelay(input_win, false);
        return;
    }

    input_data = {L"Generating table...  "};
    auto status = tabulate(fs_raw, fs_table);

    if (status == ReturnStatus::Interrupted) {
        AddOutput(L"", LineType::BlankLine);
        AddOutput(L" User interrupted table generation", LineType::OutputError);
        AddOutput(L"", LineType::BlankLine);
        if (query_result_count > 0) {
            AddOutput(L" " + std::to_wstring(query_result_count)
                + L" results in " + std::to_wstring(query_execution_time.count()) + L"ms", LineType::OutputOk);
            AddOutput(L" Results in TSV format: " + path_raw.wstring(), LineType::OutputOk);
        } else {
            AddOutput(L" Partial results in TSV format: " + path_raw.wstring(), LineType::OutputOk);
        }
        AddOutput(L" Ctrl+L to open pager (inside pager: Q to exit, H for help)", LineType::OutputOk);

        last_output_tmp_file = path_raw;

        input_data = {L""};
        while (wget_wch(input_win, &throw_away_input) != ERR);
        nodelay(input_win, false);
        return;
    } else if (status == ReturnStatus::Error) {
        AddOutput(L"", LineType::BlankLine);
        AddOutput(L" Error during table generation", LineType::OutputError);

        input_data = {L""};
        while (wget_wch(input_win, &throw_away_input) != ERR);
        nodelay(input_win, false);
        return;
    }

    while (wget_wch(input_win, &throw_away_input) != ERR);
    nodelay(input_win, false);

    fs_table.clear();
    fs_table.flush();
    fs_table.seekg(0, std::ios_base::beg);
    if (fs_table.fail()) {
        AddOutput(L" Could not seek " + path_table.wstring(), LineType::OutputError);
        input_data = {L""};
        return;
    }

    constexpr size_t buffer_size = 1024*1024;
    char buffer[buffer_size];
    int line_count = 0;

    std::vector<std::wstring> result;

    AddOutput(L"", LineType::BlankLine);
    while (fs_table.getline(buffer, buffer_size)) {
        line_count++;
        if (line_count > max_table_lines) {
            break;
        }
        AddOutput(string_to_wstring(buffer), LineType::OutputOk);
    }


    AddOutput(L"", LineType::BlankLine);
    if (query_result_count > 0) {
        AddOutput(L" " + std::to_wstring(query_result_count)
            + L" results in " + std::to_wstring(query_execution_time.count())
            + L"ms", LineType::OutputOk);
        AddOutput(L" Results in table format: " + path_table.wstring(), LineType::OutputOk);
    } else {
        AddOutput(L" Partial results in table format: " + path_table.wstring(), LineType::OutputOk);
    }
    AddOutput(L" Ctrl+L to open pager (inside pager: Q to exit, H for help)", LineType::OutputOk);

    last_output_tmp_file = path_table;
    input_data = {L""};
}


void CLI::HandleMouse() {
    MEVENT event;

    if (getmouse(&event) == ERR) {
        return;
    }

    if (event.bstate & BUTTON4_PRESSED) {
        if (wenclose(output_win, event.y, event.x)) {
            if (event.bstate & BUTTON_SHIFT) {
                output_offset_x -= 3;
            } else {
                output_offset_y += 2;
            }
        } else if (wenclose(input_win, event.y, event.x)) {
            if (event.bstate & BUTTON_SHIFT) {
                input_offset_x -= 1;
            } else {
                input_offset_y -= 1;
            }
        }
    } else if (event.bstate & BUTTON5_PRESSED) {
        if (wenclose(output_win, event.y, event.x)) {
            if (event.bstate & BUTTON_SHIFT) {
                output_offset_x += 3;
            } else {
                output_offset_y -= 2;
            }
        } else if (wenclose(input_win, event.y, event.x)) {
            if (event.bstate & BUTTON_SHIFT) {
                input_offset_x += 1;
            } else {
                input_offset_y += 1;
            }
        }
    }

    input_offset_x = std::clamp(input_offset_x, 0, static_cast<int>(input_data[cursor_y].size()));
    input_offset_y = std::clamp(input_offset_y, 0, static_cast<int>(input_data.size()) - 1);

    cursor_x = std::clamp(cursor_x, input_offset_x, input_offset_x + getmaxx(input_win));
    cursor_y = std::clamp(cursor_y, input_offset_y, input_offset_y + getmaxy(input_win));
}


void CLI::HandleKeyTab() {
    auto& line = input_data[cursor_y];
    line.insert(line.begin() + cursor_x, indentation, ' ');
    cursor_x += indentation;
}


void CLI::HandleNormalChars(const wchar_t ch) {
    if (ch < ' ') {
        return;
    }
    auto& line = input_data[cursor_y];
    line.insert(line.begin() + cursor_x, ch);
    cursor_x++;
}


void CLI::HandleBackspaceWord() {
    if (term_name == "xterm") {
        HandleKeyBackspace();
        return;
    }

    if (cursor_x > 0) {
        auto& line = input_data[cursor_y];
        int beg = cursor_x;
        if (beg > 0) {
            beg--;
        }
        while (beg > 0
            && ((line[beg] == ' ' || line[beg] == '\t')
                || (line[beg - 1] != ' ' && line[beg - 1] != '\t')))
        {
            beg--;
        }
        line.erase(beg, cursor_x-beg);
        cursor_x = beg;
    } else if (cursor_x == 0 && cursor_y > 0) {
        cursor_x = input_data[cursor_y-1].size();
        input_data[cursor_y-1] += std::move(input_data[cursor_y]);
        input_data.erase(input_data.begin() + cursor_y);
        cursor_y--;
    }
}


void CLI::HandleWordLeft() {
    if (cursor_x == 0 && cursor_y > 0) {
        cursor_y--;
        cursor_x = input_data[cursor_y].size();
    }

    auto& line = input_data[cursor_y];
    if (cursor_x > 0) {
        cursor_x--;
    }
    while (cursor_x > 0
           && ((line[cursor_x] == ' ' || line[cursor_x] == '\t')
               || (line[cursor_x - 1] != ' ' && line[cursor_x - 1] != '\t')))
    {
        cursor_x--;
    }
}


void CLI::HandleWordRight() {
    if (cursor_x == static_cast<int>(input_data[cursor_y].size())
        && cursor_y + 1 < static_cast<int>(input_data.size()))
    {
        cursor_y++;
        cursor_x = 0;
    }

    auto& line = input_data[cursor_y];
    if (cursor_x < static_cast<int>(line.size())) {
        cursor_x++;
    }
    while (cursor_x < static_cast<int>(line.size())
           && ((line[cursor_x] != ' ' && line[cursor_x] != '\t')
               || (cursor_x > 0 && (line[cursor_x - 1] == ' ' || line[cursor_x - 1] == '\t'))))
    {
        cursor_x++;
    }
}


void CLI::HandleJoinLines() {
    if (cursor_y + 1 >= static_cast<int>(input_data.size())) {
        return;
    }
    auto& current_line = input_data[cursor_y];
    auto& next_line = input_data[cursor_y + 1];

    auto pos = current_line.find_last_not_of(' ');
    if (pos != std::wstring::npos && pos + 1 < current_line.size()) {
        current_line.erase(pos + 1);
    }

    current_line += ' ';

    pos = next_line.find_first_not_of(' ');
    if (pos == std::wstring::npos) {
        pos = next_line.size();
    }

    current_line += next_line.substr(pos);

    input_data.erase(input_data.begin() + cursor_y + 1);
}


void CLI::HandleDeleteLine() {
    if (input_data.size() == 1) {
        input_data[0].clear();
    } else {
        input_data.erase(input_data.begin() + cursor_y);
    }
}


void CLI::HandlePrevHistory() {
    moving_through_history = true;
    moved_through_history = true;
    if (current_history_entry > 0) {
        current_history_entry--;
        input_data = input_history[current_history_entry];
        cursor_y = input_data.size() - 1;
        cursor_x = input_data[cursor_y].size();
        input_offset_y = 0;
        input_offset_x = 0;
    }
}


void CLI::HandleNextHistory() {
    moving_through_history = true;
    moved_through_history = true;
    if (current_history_entry + 1 < static_cast<int>(input_history.size())) {
        current_history_entry++;
        input_data = input_history[current_history_entry];
        cursor_y = input_data.size() - 1;
        cursor_x = input_data[cursor_y].size();
        input_offset_y = 0;
        input_offset_x = 0;
    }
}


void CLI::UpdateHistory() {
    moving_through_history = moved_through_history;
    if (!moved_through_history) {
        current_history_entry = input_history.size();
    }
    moved_through_history = false;
}


void CLI::UpdateWindows() {
    int input_height = std::min(LINES / 3, static_cast<int>(input_data.size()));
    int output_height = LINES - input_height - 1;

    wresize(output_win,  output_height, COLS);
    wresize(divider_win, 1,             COLS);
    wresize(input_win,   input_height,  COLS - prompt_start.size());
    wresize(prompt_win,  input_height,  prompt_start.size());

    mvwin(divider_win, LINES - input_height - 1, 0);
    mvwin(input_win,   LINES - input_height,     prompt_start.size());
    mvwin(prompt_win,  LINES - input_height,     0);

    werase(output_win);
    werase(input_win);
    werase(prompt_win);
}


void CLI::UpdateOffsets() {
    cursor_y = std::clamp(cursor_y, 0, static_cast<int>(input_data.size()) - 1);
    cursor_x = std::clamp(cursor_x, 0, static_cast<int>(input_data[cursor_y].size()));

    auto input_min_offset_x = std::max(0, cursor_x - getmaxx(input_win));
    input_offset_x = std::clamp(input_offset_x, input_min_offset_x, cursor_x);

    auto input_min_offset_y = std::max(0, cursor_y - getmaxy(input_win));
    auto input_max_offset_y = std::max(0, static_cast<int>(input_data.size()) - (getmaxy(input_win) + 1));
    input_max_offset_y = std::min(cursor_y, input_max_offset_y);
    input_offset_y = std::clamp(input_offset_y, input_min_offset_y, input_max_offset_y);

    auto output_max_offset_x = std::max(0, static_cast<int>(output_max_width) - (getmaxx(output_win) + 1));
    output_offset_x = std::clamp(output_offset_x, 0, output_max_offset_x);

    auto output_max_offset_y = std::max(0, static_cast<int>(output_lines.size()) - (getmaxy(output_win) + 2));
    output_offset_y = std::clamp(output_offset_y, 0, output_max_offset_y);
}


void CLI::UpdateScreen() {
    int total_button_width = 0;
    for (auto& button : buttons) {
        total_button_width += button.size() + 1;
    }
    auto button_start = (getmaxx(divider_win) + 1) - total_button_width;
    mvwhline_set(divider_win, 0, 0, 0, (getmaxx(divider_win) + 1));

    wattrset(divider_win, COLOR_PAIR(CP_PRIMARY) | WA_REVERSE);
    for (auto& button : buttons) {
        mvwaddnwstr(divider_win, 0, button_start, button.c_str(), button.size());
        button_start += button.size() + 1;
    }
    wattrset(divider_win, WA_NORMAL);


    wattrset(prompt_win, COLOR_PAIR(CP_PRIMARY) | WA_BOLD);
    mvwaddnwstr(prompt_win, 0, 0, prompt_start.c_str(), prompt_start.size());
    for (int i = 1; i <= getmaxy(prompt_win); i++) {
        mvwaddnwstr(prompt_win, i, 0, prompt_cont.c_str(), prompt_cont.size());
    }
    wattrset(prompt_win, COLOR_PAIR(CP_NORMAL));

    for (int i = 0; i <= getmaxy(input_win); i++) {
        if (i + input_offset_y >= static_cast<int>(input_data.size())) {
            break;
        }

        auto& line = input_data[i+input_offset_y];
        if (static_cast<int>(line.size()) > input_offset_x) {
            mvwaddnwstr(input_win, i, 0, line.c_str() + input_offset_x, getmaxx(input_win) + 1);
        }
    }

    for (int i = getmaxy(output_win); i >= 0; i--) {
        auto hist_idx = static_cast<int>(output_lines.size()) - getmaxy(output_win) - 1 + i - output_offset_y;

        if (hist_idx < 0 || hist_idx >= static_cast<int>(output_lines.size())) {
            continue;
        }

        auto& line = output_lines[hist_idx];
        if (output_offset_x >= static_cast<int>(line.size())) {
            continue;
        }
        PrintToWindow(line, output_win, i, output_offset_x);
        // mvwaddnwstr(output_win, i, 0, line.c_str() + output_offset_x, COLS);
    }

    if (wmove(input_win, cursor_y - input_offset_y, cursor_x - input_offset_x) != OK) {
        AddOutput(L" Cursor outside input window", LineType::OutputError);
    }

    // On native macOS screen corruption occurs and the terminal does not update
    // properly. Adding clearok to force a full refresh every time fixes the issue.
    // However, this is less efficient than only refreshing changes. If the
    // overhead causes problems we should consider only enabling this on macOS.
    clearok(output_win, TRUE);

    // Order of refresh matters, last window sets the terminal cursor
    wnoutrefresh(output_win);
    wnoutrefresh(divider_win);
    wnoutrefresh(prompt_win);
    wnoutrefresh(input_win);

    doupdate();
}


void CLI::Update() {
    UpdateHistory();
    UpdateWindows();
    UpdateOffsets();
    UpdateScreen();
}


void CLI::update_process_spinner() {
    if (input_data.back().empty()) {
        input_data.back() += process_spinner[process_spinner_idx];
    } else {
        input_data.back().back() = process_spinner[process_spinner_idx];
    }
    process_spinner_idx = (process_spinner_idx + 1) % process_spinner.size();
    cursor_x = input_data.back().size();
    Update();
}


ReturnStatus CLI::calculate_column_widths(std::istream& input_stream, std::vector<int>& column_widths) {
    auto input_buffer = BufferedReader(input_stream);

    int  current_column = 0;
    int  current_width  = 0;
    int  expected_continuation_bytes;

    uint64_t counter = 0;
    auto last_time = chrono::system_clock::now();

    while (true) {
        if (counter++ % 1'000'000 == 0) {
            if (last_time + chrono::milliseconds(100) < chrono::system_clock::now()) {
                update_process_spinner();
                last_time = chrono::system_clock::now();
            }
            wint_t input;
            int input_type = wget_wch(input_win, &input);
            if (input_type == OK && input == CHAR_CTRL_C) {
                return ReturnStatus::Interrupted;
            }
        }

        auto current_char = input_buffer.get();

        if (input_buffer.end) {
            return ReturnStatus::Ok;
        }

        if ((current_char & UTF_1_PREFIX_MASK) == UTF_1_PREFIX) {
            if (current_char == '\n' || current_char == '\t') {
                if (current_column >= static_cast<int>(column_widths.size())) {
                    column_widths.push_back(current_width);
                } else {
                    column_widths[current_column] = std::max(column_widths[current_column], current_width);
                }
                current_width = 0;
                if (current_char == '\n') {
                    current_column = 0;
                } else {
                    current_column++;
                }
                continue;
            } else {
                current_width++;
                continue;
            }
        }
        else if ((current_char & UTF_2_PREFIX_MASK) == UTF_2_PREFIX) {
            expected_continuation_bytes = 1;
        }
        else if ((current_char & UTF_3_PREFIX_MASK) == UTF_3_PREFIX) {
            expected_continuation_bytes = 2;
        }
        else if ((current_char & UTF_4_PREFIX_MASK) == UTF_4_PREFIX) {
            expected_continuation_bytes = 3;
        }
        else {
            current_width++;
            continue;
        }

        for (int i = 0; i < expected_continuation_bytes; i++) {
            current_char = input_buffer.get();

            if (input_buffer.end) {
                return ReturnStatus::Error;
            }
            if ((current_char & UTF_CONT_PREFIX_MASK) != UTF_CONT_PREFIX) {
                current_width += 2;
                continue;
            }
        }

        current_width++;
    }
}


ReturnStatus CLI::tabulate(std::istream& input_stream, std::ostream& output_stream) {
    auto input = BufferedReader(input_stream);
    auto output = BufferedWriter(output_stream);

    std::vector<int> column_widths;
    auto status = calculate_column_widths(input_stream, column_widths);

    if (status != ReturnStatus::Ok) {
        return status;
    }

    auto top_divider = make_divider(column_widths, 1, hline, top_left, top_mid, top_right);
    auto mid_divider = make_divider(column_widths, 1, hline, mid_left, mid_mid, mid_right);
    auto bot_divider = make_divider(column_widths, 1, hline, bot_left, bot_mid, bot_right);

    auto separator = ' ' + vline + ' ';
    auto line_end = ' ' + vline + '\n';

    input_stream.clear();
    input_stream.seekg(0, std::ios_base::beg);
    if (input_stream.fail()) {
        return ReturnStatus::Error;
    }

    int  current_column = 0;
    int  current_width  = 0;
    bool first_line     = true;
    int  expected_continuation_bytes;

    output.write(top_divider.c_str(), top_divider.size());
    output.write(separator.c_str(), separator.size());

    uint64_t counter = 0;
    auto last_time = chrono::system_clock::now();

    while (true) {
        if (counter++ % 1'000'000 == 0) {
            if (last_time + chrono::milliseconds(100) < chrono::system_clock::now()) {
                update_process_spinner();
                last_time = chrono::system_clock::now();
            }
            wint_t input;
            int input_type = wget_wch(input_win, &input);
            if (input_type == OK && input == CHAR_CTRL_C) {
                return ReturnStatus::Interrupted;
            }
        }

        if (output.fail) {
            return ReturnStatus::Error;
        }

        auto current_char = input.get();

        if (input.end) {
            output.write(bot_divider.c_str(), bot_divider.size());
            return ReturnStatus::Ok;
        }

        if ((current_char & UTF_1_PREFIX_MASK) == UTF_1_PREFIX) {
            if (current_char == '\n') {
                auto spaces = column_widths[current_column] - current_width;
                while (spaces-- > 0) output.put(' ');

                output.write(line_end.c_str(), line_end.size());


                if (first_line) {
                    first_line = false;
                    output.write(mid_divider.c_str(), mid_divider.size());
                }

                if (input.has_more()) {
                    output.write(separator.c_str(), separator.size());
                }
                current_column = 0;
                current_width = 0;
                continue;
            }
            else if (current_char == '\t') {
                auto spaces = column_widths[current_column] - current_width;
                while (spaces-- > 0) output.put(' ');

                output.write(separator.c_str(), separator.size());
                current_column++;
                current_width = 0;
                continue;
            }
            else {
                output.put(current_char);
                current_width++;
                continue;
            }
        }
        else if ((current_char & UTF_2_PREFIX_MASK) == UTF_2_PREFIX) {
            expected_continuation_bytes = 1;
        }
        else if ((current_char & UTF_3_PREFIX_MASK) == UTF_3_PREFIX) {
            expected_continuation_bytes = 2;
        }
        else if ((current_char & UTF_4_PREFIX_MASK) == UTF_4_PREFIX) {
            expected_continuation_bytes = 3;
        }
        else {
            output.put(current_char);
            current_width++;
            continue;
        }

        output.put(current_char);

        for (int i = expected_continuation_bytes - 1; i >= 0; i--) {
            current_char = input.get();
            if (input.end) {
                return ReturnStatus::Error;
            }
            output.put(current_char);
        }

        current_width++;
    }
}


void CLI::ProcessSPARQLQuery(std::ostream& os, const std::string& query) {
    try {
        auto start_time = chrono::system_clock::now();
        QueryContext::set_query_ctx(&qc);

        auto version_scope = buffer_manager.init_version_readonly();
        get_query_ctx().prepare(*version_scope, timeout);

        std::unique_ptr<SPARQL::Op> logical_plan;
        logical_plan = SPARQL::QueryParser::get_query_plan(query);

        auto query_optimizer = SPARQL::ExecutorConstructor(SPARQL::ResponseType::TSV);
        logical_plan->accept_visitor(query_optimizer);
        auto physical_plan = std::move(query_optimizer.executor);

        query_result_count = physical_plan->execute(os);
        query_execution_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start_time);
        query_return_status = ReturnStatus::Ok;
        query_is_done = true;
        return;
    } catch (QueryParsingException& e) {
        size_t line = 0;
        int line_start = -1;
        size_t line_end = query.size();

        for (size_t pos = 0; pos < query.size(); pos++) {
            if (line_start == -1 && line == e.line - 1) {
                line_start = pos;
            }
            if (line_end == query.size() && line == e.line) {
                line_end = pos;
                break;
            }
            if (query[pos] == '\n') {
                line++;
            }
        }

        AddOutput(L"", LineType::BlankLine);
        std::wstringstream ss;
        ss << " Parsing error at " << e.line << ":" << e.column + 1 << ": " << e.what();
        AddOutput(ss.str(), LineType::OutputError);

        if (line_start >= 0 && line_end <= query.size()) {
            auto line = query.substr(line_start, line_end - line_start);
            auto wline = string_to_wstring(line);
            AddOutput(std::wstring(prompt_start.size() + e.column, ' ') + L'v', LineType::OutputError);
            AddOutput(std::wstring(prompt_start.size(), ' ') + wline, LineType::OutputOk);
        }
        query_result_count = -1;
        query_return_status = ReturnStatus::Error;
        query_is_done = true;
        return;
    } catch (InterruptedException& e) {
        query_result_count = -1;
        query_return_status = ReturnStatus::Interrupted;
        query_is_done = true;
        return;
    } catch (std::exception& e) {
        AddOutput(L"", LineType::BlankLine);
        AddOutput(L' ' + string_to_wstring(e.what()), LineType::OutputError);
        query_result_count = -1;
        query_return_status = ReturnStatus::Error;
        query_is_done = true;
        return;
    }
}


void CLI::ProcessMQLQuery(std::ostream& os, const std::string& query) {
    try {
        auto start_time = chrono::system_clock::now();
        QueryContext::set_query_ctx(&qc);

        auto version_scope = buffer_manager.init_version_readonly();
        get_query_ctx().prepare(*version_scope, timeout);

        std::unique_ptr<MQL::Op> logical_plan;
        logical_plan = MQL::QueryParser::get_query_plan(query);

        auto query_optimizer = MQL::ExecutorConstructor(MQL::ReturnType::TSV);
        logical_plan->accept_visitor(query_optimizer);
        auto physical_plan = std::move(query_optimizer.executor);

        query_result_count = physical_plan->execute(os);
        query_execution_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start_time);
        query_return_status = ReturnStatus::Ok;
        query_is_done = true;
        return;
    } catch (QueryParsingException& e) {
        size_t line = 0;
        int line_start = -1;
        size_t line_end = query.size();

        for (size_t pos = 0; pos < query.size(); pos++) {
            if (line_start == -1 && line == e.line - 1) {
                line_start = pos;
            }
            if (line_end == query.size() && line == e.line) {
                line_end = pos;
                break;
            }
            if (query[pos] == '\n') {
                line++;
            }
        }

        AddOutput(L"", LineType::BlankLine);
        std::wstringstream ss;
        ss << " Parsing error at " << e.line << ":" << e.column + 1 << ": " << e.what();
        AddOutput(ss.str(), LineType::OutputError);

        if (line_start >= 0 && line_end <= query.size()) {
            auto line = query.substr(line_start, line_end - line_start);
            auto wline = string_to_wstring(line);
            AddOutput(std::wstring(prompt_start.size() + e.column, ' ') + L'v', LineType::OutputError);
            AddOutput(std::wstring(prompt_start.size(), ' ') + wline, LineType::OutputOk);
        }
        query_result_count = -1;
        query_return_status = ReturnStatus::Error;
        query_is_done = true;
        return;
    } catch (InterruptedException& e) {
        query_result_count = -1;
        query_return_status = ReturnStatus::Interrupted;
        query_is_done = true;
        return;
    } catch (std::exception& e) {
        AddOutput(L"", LineType::BlankLine);
        AddOutput(L' ' + string_to_wstring(e.what()), LineType::OutputError);
        query_result_count = -1;
        query_return_status = ReturnStatus::Error;
        query_is_done = true;
        return;
    }
}

void CLI::ProcessGQLQuery(std::ostream& os, const std::string& query) {
    try {
        auto start_time = chrono::system_clock::now();
        QueryContext::set_query_ctx(&qc);

        auto version_scope = buffer_manager.init_version_readonly();
        get_query_ctx().prepare(*version_scope, timeout);

        std::unique_ptr<GQL::Op> logical_plan;
        logical_plan = GQL::QueryParser::get_query_plan(query);

        auto query_optimizer = GQL::ExecutorConstructor(GQL::ReturnType::TSV);
        logical_plan->accept_visitor(query_optimizer);
        auto physical_plan = std::move(query_optimizer.executor);

        query_result_count = physical_plan->execute(os);
        query_execution_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start_time);
        query_return_status = ReturnStatus::Ok;
        query_is_done = true;
        return;
    } catch (QueryParsingException& e) {
        size_t line = 0;
        int line_start = -1;
        size_t line_end = query.size();

        for (size_t pos = 0; pos < query.size(); pos++) {
            if (line_start == -1 && line == e.line - 1) {
                line_start = pos;
            }
            if (line_end == query.size() && line == e.line) {
                line_end = pos;
                break;
            }
            if (query[pos] == '\n') {
                line++;
            }
        }

        AddOutput(L"", LineType::BlankLine);
        std::wstringstream ss;
        ss << " Parsing error at " << e.line << ":" << e.column + 1 << ": " << e.what();
        AddOutput(ss.str(), LineType::OutputError);

        if (line_start >= 0 && line_end <= query.size()) {
            auto line = query.substr(line_start, line_end - line_start);
            auto wline = string_to_wstring(line);
            AddOutput(std::wstring(prompt_start.size() + e.column, ' ') + L'v', LineType::OutputError);
            AddOutput(std::wstring(prompt_start.size(), ' ') + wline, LineType::OutputOk);
        }
        query_result_count = -1;
        query_return_status = ReturnStatus::Error;
        query_is_done = true;
        return;
    } catch (InterruptedException& e) {
        query_result_count = -1;
        query_return_status = ReturnStatus::Interrupted;
        query_is_done = true;
        return;
    } catch (std::exception& e) {
        AddOutput(L"", LineType::BlankLine);
        AddOutput(L' ' + string_to_wstring(e.what()), LineType::OutputError);
        query_result_count = -1;
        query_return_status = ReturnStatus::Error;
        query_is_done = true;
        return;
    }
}


int CLI::MainLoop() {
    while (true) {
        Update();

        wint_t input;
        int input_type = wget_wch(input_win, &input);

        if (input_type == OK) {
            if      (input == CHAR_ENTER)          { HandleKeyEnter(); }
            else if (input == CHAR_CTRL_S)         { HandleSend(); }
            else if (input == CHAR_CTRL_P)         { HandlePrevHistory(); }
            else if (input == CHAR_CTRL_N)         { HandleNextHistory(); }
            else if (input == CHAR_CTRL_H)         { HandleBackspaceWord(); }
            else if (input == CHAR_CTRL_C)         { input_data = {L""}; }
            else if (input == CHAR_CTRL_O)         { output_lines.clear(); output_max_width = 0; }
            else if (input == CHAR_CTRL_D)         { return EXIT_SUCCESS; }
            else if (input == CHAR_CTRL_L)         { ShowPager(); }
            else if (input == CHAR_CTRL_U)         { HandleJoinLines(); }
            else if (input == CHAR_CTRL_K)         { HandleDeleteLine(); }
            else if (input == CHAR_TAB)            { HandleKeyTab(); }
            else if (input == CHAR_DEL)            { HandleKeyBackspace(); }
            else                                   { HandleNormalChars(input); }
        }
        else if (input_type == KEY_CODE_YES) {
            if      (input == KEY_RESIZE)          { endwin(); Init(); Update(); }
            else if (input == KEY_MOUSE)           { HandleMouse(); }
            else if (input == KEY_DC)              { HandleKeyDelete(); }
            else if (input == KEY_F(1))            { ShowHelp(); }
            else if (input == KEY_UP)              { HandleKeyUp(); }
            else if (input == KEY_DOWN)            { HandleKeyDown(); }
            else if (input == KEY_LEFT)            { HandleKeyLeft(); }
            else if (input == KEY_RIGHT)           { HandleKeyRight(); }
            else if (input == KEY_BACKSPACE)       { HandleKeyBackspace(); }
            else if (input == KEY_HOME)            { cursor_x = 0; }
            else if (input == KEY_END)             { cursor_x = input_data[cursor_y].size(); }
            else if (input == KEY_PPAGE)           { output_offset_y += getmaxy(output_win) + 1; }
            else if (input == KEY_NPAGE)           { output_offset_y -= getmaxy(output_win) + 1; }
            else if (input == KEY_SLEFT)           { output_offset_x -= 2; }
            else if (input == KEY_SRIGHT)          { output_offset_x += 2; }
            else if (input == KEY_CODE_SHIFT_UP)   { output_offset_y += 2; }
            else if (input == KEY_CODE_SHIFT_DOWN) { output_offset_y -= 2; }
            else if (input == KEY_CODE_CTRL_UP)    { HandleKeyUp(); }
            else if (input == KEY_CODE_CTRL_DOWN)  { HandleKeyDown(); }
            else if (input == KEY_CODE_CTRL_LEFT)  { HandleWordLeft(); }
            else if (input == KEY_CODE_CTRL_RIGHT) { HandleWordRight(); }
            else if (input == KEY_CODE_CTRL_HOME)  { cursor_x = cursor_y = 0; }
            else if (input == KEY_CODE_CTRL_END)   { cursor_y = input_data.size() - 1; cursor_x = input_data[cursor_y].size(); }
        }
    }
}
} // namespace Tui


int RunCLI(Model model, chrono::seconds timeout) {
    auto tui = CLI::CLI(model, timeout);

    auto ret = tui.Init();
    if (ret != EXIT_SUCCESS) {
        return  ret;
    }

    tui.ShowHelp();
    tui.ReadHistoryFile();

    return tui.MainLoop();
}

#endif
