#include <iostream>

#include "hash_trie.h"
#include "trie.h"

using namespace std;


// Build Trie
int main() {
    // Initialize tree
    auto tree = Trie();
    //auto tree = HashTrie();

    // Values to insert
    auto values = vector<pair<uint64_t, uint64_t>>();
    values.push_back(make_pair(1, 2));
    values.push_back(make_pair(1, 3));
    values.push_back(make_pair(1, 4));
    values.push_back(make_pair(1, 4));
    values.push_back(make_pair(2, 3));
    values.push_back(make_pair(2, 5));
    values.push_back(make_pair(3, 1));
    values.push_back(make_pair(5, 10));
    values.push_back(make_pair(5, 12));

    // Insertion
    for (auto element : values) {
        tree.insert(element.first, element.second);
    }

    // Prepare tree
    tree.end_inserts();

    // Print final tree
    tree.print();
    tree.print_offsets();

    // Query the tree
    cout << "Results: \n" << endl;
    for (auto results = tree.query(1); results.first < results.second; results.first++) {
        cout << *results.first << endl;
    }
}