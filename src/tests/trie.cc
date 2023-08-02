/*
There are four different tests, one for each type of search:
- Exact Search
- Prefix Search
- Error Exact Search
- Error Prefix Search

We start by inserting the words (words.txt) into the trie and then querying for each one of the searches.
Then, the results are sorted in alphabetical order and compared with the results files that
were calculated by brute force.

Queries Files:
- search_queries.txt (Normal searches)
- error_search_queries.txt (Error searches)

Results Files:
- exact_search_results.txt
- prefix_search_results.txt
- error_exact_search_results.txt
- error_prefix_search_results.txt

* All files are in "tests/trie/"
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>

#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/filesystem.h"
#include "storage/index/text_search/trie.h"

void normalizeString(std::string& str) {
    // To lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

static const std::string DIR = "../../../tests/trie";

int main() {

    // check if db_folder does not exists
    std::string db_folder = DIR + "/tmp-db";
    {
        if (Filesystem::exists(db_folder)) {
            std::filesystem::remove_all(db_folder);
        }
    }

    // Init of FileManager and BufferManager
    FileManager::init(db_folder);
    BufferManager::init(BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE, 0, 0);

    {
        // Init Trie
        Trie trie;

        std::vector<std::string> words;

        // Input file (Insert words)
        std::ifstream input_file;
        input_file.open(DIR + "/words.txt");
        std::string word;
        uint64_t id = 1;
        while (std::getline(input_file, word)) {
            // Normalize word
            words.push_back(word);
            normalizeString(word);

            // Insertion of normalized word in Trie
            trie.add_new_word(word, id);
            id++;
        }

        // Test 1: Exact Search
        std::ifstream search_file1;
        search_file1.open(DIR + "/search_queries.txt");
        std::ifstream results_file1;
        results_file1.open(DIR + "/Results/exact_search_results.txt");
        while (std::getline(search_file1, word)) {
            std::vector<std::string> responses;
            normalizeString(word);
            auto it = trie.get_exact_iter(word);
            for (auto id = it.next(); id != 0; id = it.next()) {
                responses.push_back(words[id-1]);
            }
            std::sort(std::begin(responses), std::end(responses));

            std::vector<std::string> results;
            std::string line;
            std::getline(results_file1, line);
            std::stringstream stream(line);
            std::string item;
            while (stream >> item) results.push_back(item);
            std::sort(std::begin(results), std::end(results));

            if (responses.size() != results.size()) {
                std::cout << "FAIL TEST 1: Exact Search:" << std::endl;
                std::cout << "Searched word: " << word << std::endl;
                std::cout << "Number of results Trie: " << responses.size() << std::endl;
                std::cout << "Number of results Test: " << results.size() << std::endl;
                return 1;
            }

            for (size_t i = 0; i < responses.size(); i++)
            {
                if (strcmp(responses[i].c_str(), results[i].c_str())) {
                    std::cout << "FAIL TEST 1: Exact Search:" << std::endl;
                    std::cout << "Searched word: " << word << std::endl;
                    std::cout << "Number of results Trie: " << responses.size() << std::endl;
                    std::cout << "Number of results Test: " << results.size() << std::endl;
                    std::cout << "Trie: " << responses[i] << "\nTest: " << results[i] << std::endl;
                    return 1;
                }
            }
        }
        std::cout << "TEST 1 (Exact Search) CORRECT" << std::endl;

        // Test 2: Prefix Search
        std::ifstream search_file2;
        search_file2.open(DIR + "/search_queries.txt");
        std::ifstream results_file2;
        results_file2.open(DIR + "/Results/prefix_search_results.txt");
        while (std::getline(search_file2, word)) {
            std::vector<std::string> responses;
            normalizeString(word);
            auto it = trie.get_iter(word);
            for (auto id = it.next(); id != 0; id = it.next()) {
                responses.push_back(words[id-1]);
            }
            std::sort(std::begin(responses), std::end(responses));

            std::vector<std::string> results;
            std::string line;
            std::getline(results_file2, line);
            std::stringstream stream(line);
            std::string item;
            while (stream >> item) results.push_back(item);
            std::sort(std::begin(results), std::end(results));

            if (responses.size() != results.size()) {
                std::cout << "FAIL TEST 2: Prefix Search:" << std::endl;
                std::cout << "Searched word: " << word << std::endl;
                std::cout << "Number of results Trie: " << responses.size() << std::endl;
                std::cout << "Number of results Test: " << results.size() << std::endl;
                return 1;
            }

            for (size_t i = 0; i < responses.size(); i++)
            {
                if (strcmp(responses[i].c_str(), results[i].c_str())) {
                    std::cout << "FAIL TEST 2: Prefix Search:" << std::endl;
                    std::cout << "Searched word: " << word << std::endl;
                    std::cout << "Number of results Trie: " << responses.size() << std::endl;
                    std::cout << "Number of results Test: " << results.size() << std::endl;
                    std::cout << "Trie: " << responses[i] << "\nTest: " << results[i] << std::endl;
                    return 1;
                }
            }
        }
        std::cout << "TEST 2 (Prefix Search) CORRECT" << std::endl;

        // Test 3: Error Exact Search
        std::ifstream search_file3;
        search_file3.open(DIR + "/error_search_queries.txt");
        std::ifstream results_file3;
        results_file3.open(DIR + "/Results/error_exact_search_results.txt");
        while (std::getline(search_file3, word)) {
            std::vector<std::string> responses;
            normalizeString(word);
            auto it = trie.get_exact_iter_error_search(word);
            for (auto id = it.next(); id != 0; id = it.next()) {
                responses.push_back(words[id-1]);
            }
            std::sort(std::begin(responses), std::end(responses));

            std::vector<std::string> results;
            std::string line;
            std::getline(results_file3, line);
            std::stringstream stream(line);
            std::string item;
            while (stream >> item) results.push_back(item);
            std::sort(std::begin(results), std::end(results));

            if (responses.size() != results.size()) {
                std::cout << "FAIL TEST 3: Error Exact Search:" << std::endl;
                std::cout << "Searched word: " << word << std::endl;
                std::cout << "Number of results Trie: " << responses.size() << std::endl;
                std::cout << "Number of results Test: " << results.size() << std::endl;
                return 1;
            }

            for (size_t i = 0; i < responses.size(); i++)
            {
                if (strcmp(responses[i].c_str(), results[i].c_str())) {
                    std::cout << "FAIL TEST 3: Error Exact Search:" << std::endl;
                    std::cout << "Searched word: " << word << std::endl;
                    std::cout << "Number of results Trie: " << responses.size() << std::endl;
                    std::cout << "Number of results Test: " << results.size() << std::endl;
                    std::cout << "Trie: " << responses[i] << "\nTest: " << results[i] << std::endl;
                    return 1;
                }
            }
        }
        std::cout << "TEST 3 (Error Exact Search) CORRECT" << std::endl;

        // Test 4: Error Prefix Search
        std::ifstream search_file4;
        search_file4.open(DIR + "/error_search_queries.txt");
        std::ifstream results_file4;
        results_file4.open(DIR + "/Results/error_prefix_search_results.txt");
        while (std::getline(search_file4, word)) {
            std::vector<std::string> responses;
            normalizeString(word);
            auto it = trie.get_iter_error_search(word);
            for (auto id = it.next(); id != 0; id = it.next()) {
                responses.push_back(words[id-1]);
            }
            std::sort(std::begin(responses), std::end(responses));

            std::vector<std::string> results;
            std::string line;
            std::getline(results_file4, line);
            std::stringstream stream(line);
            std::string item;
            while (stream >> item) results.push_back(item);
            std::sort(std::begin(results), std::end(results));

            if (responses.size() != results.size()) {
                std::cout << "FAIL TEST 4: Error Prefix Search:" << std::endl;
                std::cout << "Searched word: " << word << std::endl;
                std::cout << "Number of results Trie: " << responses.size() << std::endl;
                std::cout << "Number of results Test: " << results.size() << std::endl;
                return 1;
            }

            for (size_t i = 0; i < responses.size(); i++)
            {
                if (strcmp(responses[i].c_str(), results[i].c_str())) {
                    std::cout << "FAIL TEST 4: Error Prefix Search:" << std::endl;
                    std::cout << "Searched word: " << word << std::endl;
                    std::cout << "Number of results Trie: " << responses.size() << std::endl;
                    std::cout << "Number of results Test: " << results.size() << std::endl;
                    std::cout << "Trie: " << responses[i] << "\nTest: " << results[i] << std::endl;
                    return 1;
                }
            }
        }
        std::cout << "TEST 4 (Error Prefix Search) CORRECT" << std::endl;
    }
    
    // Destruction of BufferManager and FileManager
    buffer_manager.~BufferManager();
    file_manager.~FileManager();

    return 0;
}