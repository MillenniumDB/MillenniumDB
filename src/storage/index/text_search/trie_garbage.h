#pragma once

#include <ostream>
#include <filesystem>

#include "storage/file_id.h"
#include "storage/page/unversioned_page.h"

namespace TextSearch {

class TrieGarbage {
public:
    // File id for FileManager
    const FileId garbage_file_id;

    // The garbage is distributed as follows:
    // - Directory page, which functions as a directory for all capacities available (including nodes).
    // - And all the other pages, which store free offsets corresponding to their capacity,
    //   number of offsets stored in page and values for next and previous pages.
    //   All of these pages work as linked lists.

    // Directory page: (Only first page of garbage file)
    unsigned char* capacities_count;        // N   (1B) Number of stored capacities
    unsigned char* last_page;               // LP  (5B) Last page written
    unsigned char* first_and_last_pages;    // FLPs Section of first and last pages of each category of garbage.
                                            // Both first and last page use 4B, so each category use in total 8B in
                                            // directory.
    // Constants of sizes:
    static constexpr int DP_SN = 1;     // Size of N (Directory page)
    static constexpr int DP_SLP = 5;    // Size of LP (Directory page)
    static constexpr int DP_FLPs1 = 4;  // Size of first pages in FLPs section (Directory page)
    static constexpr int DP_FLPs2 = 4;  // Size of last pages in FLPs section (Directory page)

    // This offset is used to obtain the position of each capacity in garbage
    static constexpr int OFFSET_SLOT_CAPACITY = 3; // = (log2(Trie::CAPACITY) - 1)


    // Offsets pages:
    static constexpr int OP_SN = 2;  // Size of the number of stored offsets in page (Offsets page)
    static constexpr int OP_SPP = 4; // Size of previous page number (Offsets page)
    static constexpr int OP_SNP = 4; // Size of next page number (Offsets page)
    static constexpr int OP_SO = 5;  // Size of each offset stored in page (Offsets page)
    // Max number of offset in Offsets pages: (2B offsets count + 4B prev page + 4B next page and 5B each value)
    static constexpr uint64_t MAX_VALUES_IN_PAGE = (UPage::SIZE - (OP_SN + OP_SPP + OP_SNP)) / OP_SO;

    // Directory of garbage
    UPage& dir_page;

    // Constructor
    TrieGarbage(const std::filesystem::path& path);

    // Destructor
    ~TrieGarbage();

    // Adds value to the garbage of capacity bytes
    void add_capacity(uint64_t capacity, uint64_t value);

    // Search for a free space of >= capacity bytes and pops it.
    // Overwrites capacity (if bigger) and value to the result.
    // Returns false if nothing is found and true otherwise.
    bool search_and_pop_capacity(uint64_t& capacity, uint64_t& value);

    // Adds value to the garbage of nodes
    void add_node(uint64_t value);

    // Search for a free space of node and pops it.
    // Overwrites value to the result.
    // Returns false if nothing is found and true otherwise.
    bool search_and_pop_node(uint64_t& value);

    // Prints status of garbage
    void status(std::ostream& os);

private:

    // Function to write the value in B bytes from pos
    static inline void write_xbytes(uint64_t value, unsigned char* pos, int B) {
        for (int i = 0, shift = 0; i < B; ++i, shift += 8) {
            pos[i] = (value >> shift) & 0xFF;
        }
    }

    // Function to read B bytes from pos
    static inline uint64_t read_xbytes(unsigned char* pos, int B) {
        uint64_t value = 0;

        for (int i = 0, shift = 0; i < B; ++i, shift += 8) {
            value |= static_cast<uint64_t>(pos[i]) << shift;
        }

        return value;
    }
};

} // namespace TextSearch
