#include "graph_models/exceptions.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/fatal_error.h"
#include "query/query_context.h"
#include "storage/index/text_search/quad.h"
#include "storage/index/text_search/rdf.h"
#include "storage/index/text_search/text_search.h"
#include "storage/index/text_search/tokenize_normalize.h"
#include "storage/index/text_search/utils.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/system.h"

#include "third_party/cli11/CLI11.hpp"


bool is_valid_index_name(const std::string index_name) {
    for (auto c : index_name) {
        if (!isalnum(c) && c != '-' && c != '_') {
            return false;
        }
    }
    return true;
}


static uint64_t read_uint64(std::fstream& fs) {
    uint64_t res = 0;
    uint8_t buf[8];

    fs.read((char*)buf, sizeof(buf));

    for (int i = 0; i < 8; i++) {
        res |= static_cast<uint64_t>(buf[i]) << (i * 8);
    }

    if (!fs.good()) {
        throw std::runtime_error("Error reading uint64");
    }

    return res;
}


int main(int argc, char* argv[]) {
    std::string db_directory_path;
    std::string index_name;
    std::string predicate;
    std::string query;
    uint64_t limit = 10;

    uint64_t load_strings = StringManager::DEFAULT_LOAD_STR;
    uint64_t versioned_pages_buffer   = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    uint64_t private_pages_buffer     = BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE;
    uint64_t unversioned_pages_buffer = BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE;

    CLI::App app{"MillenniumDB Text Search"};
    app.get_formatter()->column_width(34);
    app.option_defaults()->always_capture_default();
    app.require_subcommand(1);
    app.set_help_flag();
    app.set_help_all_flag("-h,--help", "Print this help message and exit");

    app.add_option("db-directory", db_directory_path)
        ->description("Database directory")
        ->type_name("<path>")
        ->check(CLI::ExistingDirectory.description(""))
        ->required();

    app.add_option("index", index_name)
        ->description("The name of the index to use or create")
        ->type_name("<string>")
        ->required();

    app.add_option("--versioned-buffer", versioned_pages_buffer)
        ->description("Size of buffer for versioned pages shared between threads\nAllows units such as MB and GB")
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--private-buffer", private_pages_buffer)
        ->description("Size of private per-thread buffers,\nAllows units such as MB and GB")
        ->option_text("<bytes> [64MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--unversioned-buffer", unversioned_pages_buffer)
        ->description("Size of buffer for unversioned pages shared between threads,\nAllows units such as MB and GB")
        ->option_text("<bytes> [128MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--load-strings", load_strings)
        ->description("Total amount of strings to pre-load\nAllows units such as MB and GB")
        ->option_text("<bytes> [2GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));


    auto sc_index_predicate = app.add_subcommand("index-predicate", "Index a specific predicate");
    sc_index_predicate->add_option("predicate", predicate)
        ->description("Predicate to index")
        ->type_name("<IRI>")
        ->required();

    auto sc_list = app.add_subcommand("list", "List all strings that have been inserted");

    auto sc_graph = app.add_subcommand("graph", "Generate a visual representation of trie");

    auto sc_list_bpt = app.add_subcommand("list-bpt", "List all BPT contents");

    auto sc_prefix = app.add_subcommand("prefix", "List all strings matching a prefix");
    sc_prefix->add_option("prefix", query)
        ->description("Prefix to match")
        ->type_name("<string>")
        ->required();
    sc_prefix->add_option("--limit", limit)
        ->description("Limit the number of results returned")
        ->type_name("<integer>");

    auto sc_prefix_error = app.add_subcommand("prefix-error", "List all strings matching a prefix, allowing errors");
    sc_prefix_error->add_option("prefix", query)
        ->description("Prefix to match")
        ->type_name("<string>")
        ->required();
    sc_prefix_error->add_option("--limit", limit)
        ->description("Limit the number of results returned")
        ->type_name("<integer>");

    auto sc_match = app.add_subcommand("match", "List all strings matching exactly");
    sc_match->add_option("match", query)
        ->description("String to match")
        ->type_name("<string>")
        ->required();
    sc_match->add_option("--limit", limit)
        ->description("Limit the number of results returned")
        ->type_name("<integer>");

    auto sc_match_error = app.add_subcommand("match-error", "List all strings matching exactly, allowing errors");
    sc_match_error->add_option("match", query)
        ->description("String to match")
        ->type_name("<string>")
        ->required();
    sc_match_error->add_option("--limit", limit)
        ->description("Limit the number of results returned")
        ->type_name("<integer>");


    CLI11_PARSE(app, argc, argv);

    if (!is_valid_index_name(index_name)) {
        std::cerr << "Invalid index name \"" << index_name << "\". Only alphanumeric, '-' and '_' are allowed.\n";
        return EXIT_FAILURE;
    }

    if (*(db_directory_path.end() - 1) == '/') {
        db_directory_path.resize(db_directory_path.size() - 1);
    }

    auto catalog_path = db_directory_path + "/catalog.dat";
    auto catalog_fs   = std::fstream(catalog_path, std::ios::in | std::ios::binary);

    if (!catalog_fs.is_open()) {
        std::cerr << "Could not open file \"" << catalog_path << "\".\n";
        return EXIT_FAILURE;
    }

    auto model_identifier = read_uint64(catalog_fs);

    std::unique_ptr<ModelDestroyer> model_destroyer;
    TextSearch::TextSearch text_search;
    QueryContext qc;
    QueryContext::set_query_ctx(&qc);

    System system(
        db_directory_path,
        load_strings,
        versioned_pages_buffer,
        private_pages_buffer,
        unversioned_pages_buffer,
        1
    );

    try {
        switch (model_identifier) {
        case QuadCatalog::MODEL_ID: {
            model_destroyer = QuadModel::init();

            text_search = TextSearch::TextSearch(db_directory_path, 2,
                                                 &TextSearch::std_tokenize,
                                                 &TextSearch::std_normalize,
                                                 &TextSearch::Quad::index_predicate,
                                                 &TextSearch::Quad::oid_to_string);
            break;
        }
        case RdfCatalog::MODEL_ID: {
            model_destroyer = RdfModel::init();

            text_search = TextSearch::TextSearch(db_directory_path, 2,
                                                 &TextSearch::std_tokenize,
                                                 &TextSearch::std_normalize,
                                                 &TextSearch::RDF::index_predicate,
                                                 &TextSearch::RDF::oid_to_string);
            break;
        }
        default: {
            std::string error = "Unknow model identifier: " + std::to_string(model_identifier) + ". Catalog may be corrupted";
            FATAL_ERROR(error);
        }
        }
    } catch (WrongModelException& e) {
        std::string error = std::string(e.what()) + ". Catalog may be corrupted";
        FATAL_ERROR(error);
    } catch (WrongCatalogVersionException& e) {
        std::string error = std::string(e.what()) + ". The database must be created again to work with this version";
        FATAL_ERROR(error);
    }


    if (!text_search.index_exists(index_name)) {
        if (sc_index_predicate->parsed()) {
            text_search.create_index(index_name);
        } else {
            std::cerr << "Index \"" << index_name << "\" does not exist.\n";
            return EXIT_FAILURE;
        }
    }

    text_search.load_index(index_name);

    auto start = std::chrono::system_clock::now();

    if (sc_index_predicate->parsed()) {
        text_search.index_predicate(predicate);
    }
    else if (sc_list_bpt->parsed()) {

        auto [min_first, min_second] = TextSearch::compress(0, 0, 0 );
        auto [max_first, max_second] = TextSearch::compress(UINT64_MAX, UINT32_MAX, UINT64_MAX );

        Record<2> min({ min_first, min_second });
        Record<2> max({ max_first, max_second });

        auto interruption_requested = false;
        auto bpt_iter = text_search.bpt->get_range(&interruption_requested, min, max);

        while (auto record = bpt_iter.next()) {
            std::cout << (*record)[0] << ", " << (*record)[1] << ", " << (*record)[2] << "\n";
        }
    }
    else if (sc_list->parsed()) {
        auto iter = text_search.get_iter_list();
        while (iter.next()) {
            auto& str = iter.get_str();
            std::cout << str << "\n";
        }
    }
    else if (sc_graph->parsed()) {
        auto iter = text_search.get_iter_list();
        std::vector<std::string> words;
        while (iter.next()) {
            words.push_back(std::move(iter.get_str()));
        }
        auto dot_file = std::fstream("trie.dot", std::ios_base::out);
        text_search.print_trie(dot_file, std::move(words));
        dot_file.close();
        auto ec = std::system("dot -O -Tsvg trie.dot");
        if (ec != 0) {
            std::cerr << "Error ocurred trying to execute \"dot -O -Tsvg trie.dot\"\n";
            return EXIT_FAILURE;
        }
        ec = std::system("firefox trie.dot.svg");
        if (ec != 0) {
            std::cerr << "Error ocurred trying to execute \"firefox trie.dot.svg\"\n";
            return EXIT_FAILURE;
        }
    }
    else {
        std::unique_ptr<TextSearch::TextSearchIter> iter;

        if (sc_match->parsed()) {
            iter = text_search.search<TextSearch::SearchType::Match, false>(query);
        }
        else if (sc_match_error->parsed()) {
            iter = text_search.search<TextSearch::SearchType::Match, true>(query);
        }
        else if (sc_prefix->parsed()) {
            iter = text_search.search<TextSearch::SearchType::Prefix, false>(query);
        }
        else if (sc_prefix_error->parsed()) {
            iter = text_search.search<TextSearch::SearchType::Prefix, true>(query);
        } else {
            throw std::logic_error("No subcommand detected");
        }

        uint64_t count = 0;
        while(count < limit && iter->next()) {
            auto table_pointer = iter->get_table_pointer();
            auto string = text_search.get_string(table_pointer);
            std::cout << string << "\n";
            count++;
        }
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start);
    std::cout.precision(3);
    std::cout << std::fixed << "Duration: " << duration.count() / 1000.0 << "ms\n";

    return EXIT_SUCCESS;
}
