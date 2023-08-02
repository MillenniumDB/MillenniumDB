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
#include <unistd.h>

#include "storage/page.h"

class FileManager {
friend class Page; // to allow calling file_manager.flush
friend class BufferManager; // to calling file_manager.read_page
friend class StringManager; // to calling file_manager.read_page
public:
    ~FileManager() = default;

    // necessary to be called before first usage
    static void init(const std::string& db_folder);

    // Get an id for the corresponding file, creating it if it's necessary
    FileId get_file_id(const std::string& filename);

    // Create a new temporary file id
    TmpFileId get_tmp_file_id();

    // count how many pages a file have
    uint_fast32_t count_pages(FileId file_id) const {
        // We don't need mutex here as long as db is readonly
        return lseek(file_id.id, 0, SEEK_END) / Page::MDB_PAGE_SIZE;
    }

    // // delete the file represented by `tmp_file_id`, pages in private buffer using that tmp_file_id are cleared
    void remove_tmp(TmpFileId tmp_file_id);

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
    void flush(Page& page_id) const;

    // read a page from disk into memory pointed by `bytes`.
    // `bytes` must point to the start memory position of `Page::MDB_PAGE_SIZE` allocated bytes
    void read_page(PageId page_id, char* bytes) const;
};

extern FileManager& file_manager; // global object
