/*
 * FileManager maintains a list (`opened_files`) with all files that are opened,
 * and another list (`filenames`) with the string of their names.
 * The FileId its just the index, so both lists must have the same size
 * and objects at the same index are related to each other.
 *
 * All the other classes that need to work with files should use the FileManager to obtain a reference to a
 * fstream.
 *
 * This can be done with temporary or permanent files, using 'get_tmp_file_id' or 'get_file_id', in the first case, the
 * file_manager will ask for a pages in the private buffer of the specific thread, in the second case the file_manager
 * will ask for pages to the shared or public buffer.
 *
 * `file_manager` is a global object and is available when this file is included. Before using it, somebody
 * needs to call the method FileManager::init(), usually is the responsibility of the model (e.g. RelationalModel)
 * to call it.
 *
 * The instance `file_manager` cannot be destroyed before the BufferManager flushes its dirty pages on exit
 * because BufferManager needs to access the file paths from FileManager.
 */

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
#include "storage/index/tensor_store/tensor_page.h"
#include "storage/page/unversioned_page.h"
#include "storage/page/versioned_page.h"

class FileManager {
friend class Page; // to allow calling file_manager.flush
friend class BufferManager; // to allow calling file_manager.read_existing_page
friend class TensorBufferManager; // to allow calling file_manager.read_existing_page
public:
    ~FileManager() = default;

    // necessary to be called before first usage
    static void init(const std::string& db_folder);

    // Get an id for the corresponding file, creating it if it's necessary
    FileId get_file_id(const std::string& filename);

    // count how many pages a file have
    uint_fast32_t count_pages(FileId file_id) const {
        static_assert(VPage::SIZE == PPage::SIZE && VPage::SIZE == UPage::SIZE && VPage::SIZE == TensorPage::SIZE);
        // We don't need mutex here as long as db is readonly
        return lseek(file_id.id, 0, SEEK_END) / VPage::SIZE;
    }

    inline const std::string get_file_path(const std::string& filename) const noexcept {
        return db_folder + "/" + filename;
    }

private:
    // folder where all the used files will be
    const std::string db_folder;

    std::map<std::string, FileId> filename2file_id;

    // private constructor, other classes must use the global object `file_manager`
    FileManager(const std::string& db_folder);

    // page back to disk
    void flush(VPage& page) const;

    // page back to disk
    void flush(UPage& page) const;

    // page back to disk
    void flush(int fd, PPage& page) const;

    // read a tmp page from disk into memory pointed by `bytes`.
    void read_tmp_page(int fd, uint64_t page_id, char* bytes) const;

    // read a page from disk into memory pointed by `bytes`.
    void read_existing_page(PageId page_id, char* bytes) const;

    // returns the page_number of the page appended
    uint32_t append_page(FileId page_id, char* bytes) const;
};

extern FileManager& file_manager; // global object
