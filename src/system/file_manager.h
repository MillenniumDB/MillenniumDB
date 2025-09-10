/******************************************************************************
FileManager is used to work with files that are managed by the BufferManager.

This can be done with temporary or permanent files, using 'get_tmp_file_id' or
'get_file_id', in the first case, the file manager will ask for pages in the
private buffer of the specific thread, in the second case the file_manager
will ask for pages to the shared buffer.

We have a global object `file_manager` that is available when this header is
included. To use the file manager, a `System` object needs to be alive.

The instance `file_manager` cannot be destroyed before the `buffer_manager`
flushes its dirty pages on exit because the buffer manager needs to access the
file paths from the file manager.
******************************************************************************/

#pragma once

#include <map>
#include <string>

#ifdef _MSC_VER
#include <io.h>
#define lseek _lseek
#else
#include <unistd.h>
#endif

#include "storage/page/private_page.h"
#include "storage/page/page.h"

class FileManager {
    friend class BufferManager; // to allow calling read_page and update_appends
public:
    ~FileManager() = default;

    // necessary to be called before first usage
    static void init(const std::string& db_folder);

    // Get an id for the corresponding file, creating it if it's necessary
    FileId get_file_id(const std::string& filename);

    inline const std::string get_file_path(const std::string& filename) const noexcept
    {
        return db_folder + "/" + filename;
    }

private:
    // folder where all the used files will be
    const std::string db_folder;

    std::map<std::string, FileId> filename2file_id;

    // FileId -> PageCount (on disk)
    std::map<FileId, uint32_t> fid2pages;

    // private constructor, other classes must use the global object `file_manager`
    FileManager(const std::string& db_folder);

    // page back to disk
    void flush(Page& page) const;

    // page back to disk
    void flush(int fd, PPage& page) const;

    // read a tmp page from disk into memory pointed by `bytes`.
    void read_tmp_page(int fd, uint64_t page_id, char* bytes) const;

    // read a page from disk into memory pointed by `bytes`.
    void read_page(PageId page_id, char* bytes) const;

    // used after committing a write transaction, to update fid2pages
    void update_appends(const std::map<FileId, unsigned>& appended_pages);

    // count how many pages a file has
    uint32_t count_pages(FileId file_id) const;
};

extern FileManager& file_manager; // global object
