#include "text_search_index_manager.h"

#include "misc/logger.h"
#include "storage/index/text_search/text_search_index.h"
#include "system/file_manager.h"

using namespace TextSearch;

namespace fs = std::filesystem;

void TextSearchIndexManager::init(
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

    const auto text_searches_path = file_manager.get_file_path(TEXT_SEARCH_INDEX_DIR);
    if (!fs::is_directory(text_searches_path)) {
        // Text searches directory did not exist, nothing to load
        fs::create_directories(text_searches_path);
        return;
    }
}

void TextSearchIndexManager::load_text_search_index(const std::string& name, const TextSearchIndexMetadata& metadata)
{
    try {
        auto text_search_index = TextSearch::TextSearchIndex::load(
            name,
            metadata.normalization_type,
            metadata.tokenization_type,
            *this
        );
        name2text_search_index[name] = std::move(text_search_index);
        name2metadata[name] = metadata;
        predicate2names[metadata.predicate].emplace_back(name);

        logger(Category::Info) << "Loaded TextSearchIndex \"" << name << "\" with metadata:\n" << metadata;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load TextSearchIndex \"" + name + "\": " + e.what());
    } catch (...) {
        throw std::runtime_error("Failed to load TextSearchIndex \"" + name + "\": Unknown error");
    }
}

bool TextSearchIndexManager::get_text_search_index(
    const std::string& name,
    TextSearchIndex** text_search_index
)
{
    std::shared_lock lck(name2text_search_mutex);
    auto it = name2text_search_index.find(name);
    if (it == name2text_search_index.end()) {
        return false;
    }

    *text_search_index = it->second.get();
    return true;
}

std::tuple<uint_fast32_t, uint_fast32_t> TextSearchIndexManager::create_text_search_index(
    const std::string& name,
    const std::string& predicate,
    NORMALIZE_TYPE normalization_type,
    TOKENIZE_TYPE tokenization_type
)
{
    {
        std::shared_lock lck(name2text_search_mutex);
        auto it = name2text_search_index.find(name);
        if (it != name2text_search_index.end()) {
            throw std::runtime_error("TextSearchIndex \"" + name + "\" already exists");
        }
    }

    try {
        auto text_search_index = TextSearch::TextSearchIndex::create(
            name,
            normalization_type,
            tokenization_type,
            *this
        );

        auto res = text_search_index->index_predicate(predicate);

        std::unique_lock lck(name2text_search_mutex);
        name2text_search_index[name] = std::move(text_search_index);
        name2metadata[name] = { tokenization_type, normalization_type, predicate };
        predicate2names[predicate].emplace_back(name);

        has_changes_ = true;
        return res;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create TextSearchIndex \"" + name + "\": " + e.what());
    } catch (...) {
        throw std::runtime_error("Failed to create TextSearchIndex \"" + name + "\": Unknown error");
    }
}