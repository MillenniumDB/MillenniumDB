#include "text_index_manager.h"

#include <filesystem>
#include <mutex>

#include "misc/logger.h"
#include "system/file_manager.h"

using namespace TextSearch;
namespace fs = std::filesystem;

void TextIndexManager::init(
    IndexPredicateFuncType* index_predicate_func_impl_ptr_,
    IndexSingleFuncType* index_single_func_impl_ptr_,
    RemoveSingleFuncType* remove_single_func_impl_ptr_,
    ObjectIdToStringFuncType* objectid_to_string_func_ptr_
)
{
    index_predicate_func_impl_ptr = index_predicate_func_impl_ptr_;
    index_single_func_impl_ptr = index_single_func_impl_ptr_;
    remove_single_func_impl_ptr = remove_single_func_impl_ptr_;
    objectid_to_string_func_ptr = objectid_to_string_func_ptr_;

    const auto text_searches_path = file_manager.get_file_path(TEXT_SEARCH_DIR);
    if (!fs::is_directory(text_searches_path)) {
        // Text searches directory did not exist
        fs::create_directories(text_searches_path);
    }
}

void TextIndexManager::load_text_index(
    const std::string& name,
    const TextIndexMetadata& metadata
)
{
    try {
        auto text_search_index = TextSearch::TextIndex::load(
            name,
            metadata.normalization_type,
            metadata.tokenization_type,
            *this
        );
        name2text_search_index[name] = std::move(text_search_index);
        name2metadata[name] = metadata;
        predicate2names[metadata.predicate].emplace_back(name);
    } catch (const std::exception& e) {
        logger(Category::Error) << "Failed to load TextSearchIndex \"" + name + "\": " + e.what();
    } catch (...) {
        logger(Category::Error) << "Failed to load TextSearchIndex \"" + name + "\": Unknown error";
    }
}

TextIndex* TextIndexManager::get_text_index(const std::string& name)
{
    std::shared_lock lck(name2text_search_mutex);
    auto it = name2text_search_index.find(name);
    if (it == name2text_search_index.end()) {
        return nullptr;
    }

    return it->second.get();
}

std::tuple<uint_fast32_t, uint_fast32_t> TextIndexManager::create_text_search_index(
    const std::string& name,
    const std::string& predicate,
    NORMALIZE_TYPE normalization_type,
    TOKENIZE_TYPE tokenization_type
)
{
    try {
        auto text_search_index = TextSearch::TextIndex::create(
            name,
            normalization_type,
            tokenization_type,
            *this
        );

        auto&& [total_inserted_elements, total_inserted_tokens, predicate_id] = text_search_index
                                                                                    ->index_predicate(
                                                                                        predicate
                                                                                    );

        std::unique_lock lck(name2text_search_mutex);
        name2text_search_index[name] = std::move(text_search_index);
        name2metadata[name] = { tokenization_type, normalization_type, predicate_id, predicate };
        predicate2names[predicate].emplace_back(name);

        has_changes_ = true;
        return { total_inserted_elements, total_inserted_tokens };
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create TEXT index \"" + name + "\": " + e.what());
    } catch (...) {
        throw std::runtime_error("Failed to create TEXT index \"" + name + "\": Unknown error");
    }
}
