/* Page represents the content of a disk block in memory.
 * A page is treated as an array of `MDB_PAGE_SIZE` bytes (pointed by `bytes`).
 * For better performance, `MDB_PAGE_SIZE` should be multiple of the operating system's page size.
 * BufferManager is the only class who can construct a Page object. Other classes must get a Page
 * through BufferManager.
 */
#ifndef STORAGE__PAGE_H_
#define STORAGE__PAGE_H_

#include "storage/page_id.h"

class Page {
friend class BufferManager; // needed to access private constructor
public:
    static constexpr auto MDB_PAGE_SIZE = 4096;

    // contains file_id and page_number of this page
    PageId page_id;

    // mark as dirty so when page is replaced it is written back to disk.
    inline void make_dirty() noexcept { dirty = true; }

    // only meant for buffer_manager.remove()
    void reset();

    // get the start memory position of `MDB_PAGE_SIZE` allocated bytes
    inline char* get_bytes() const noexcept { return bytes; }

    // get page number
    inline uint_fast32_t get_page_number() const noexcept { return page_id.page_number; };

private:
    uint_fast32_t pins;             // count of objects using this page, modified only by buffer_manager
    char* bytes;                    // start memory address of the page, of size `MDB_PAGE_SIZE`
    bool dirty;                     // true if data in memory is different from disk

    Page() noexcept;
    Page(PageId page_id, char* bytes) noexcept;
    ~Page() = default;
    void operator=(const Page& other) noexcept;

    // write to disk if this page is dirty
    void flush();
};

#endif // STORAGE__PAGE_H_
