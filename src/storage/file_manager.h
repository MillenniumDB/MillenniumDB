/*
 * FileManager mantains a list (`opened_files`) with all files that are opened,
 * and another list (`filenames`) with the string of their names.
 * The FileId its just the index, so both lists must have the same size
 * and objects at the same index are related to each other.
 *
 * All the other clases that need to work with files should use the FileManager to obtain a reference to a
 * fstream.
 *
 * This can be done with temporary or permanent files, using 'get_tmp_file_id' or 'get_file_id', in the first case, the
 * file_manager will ask for a pages in the private buffer of the specific thread, in the second case the file_maneger
 * will ask for pages to the shared or public buffer.
 *
 * `file_manager` is a global object and is available when this file is included. Before using it, somebody
 * needs to call the method FileManager::init(), usually is the responsability of the model (e.g. RelationalModel)
 * to call it.
 *
 * The instance `file_manager` cannot be destroyed before the BufferManager flushes its dirty pages on exit
 * because BufferManager needs to access the file paths from FileManager.
 */

#ifndef STORAGE__FILE_MANAGER_H_
#define STORAGE__FILE_MANAGER_H_

#include <fstream>
#include <queue>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "storage/file_id.h"
#include "storage/page_id.h"

class FileManager {
friend class Page; // to allow calling file_manager.flush
friend class BufferManager; // to calling file_manager.read_page
public:
    ~FileManager() = default;

    // necesary to be called before first usage
    static void init(const std::string& db_folder);

    // Get an id for the corresponding file, creating it if it's necessary
    FileId get_file_id(const std::string& filename);

    // Create a new temporary file id
    TmpFileId get_tmp_file_id();

    // get the file stream assignated to `file_id` as a reference. Only use this when not accessing via BufferManager
    std::fstream& get_file(const FileId file_id) const;

    // count how many pages a file have
    uint_fast32_t count_pages(const FileId file_id) const;

    // delete the file represented by `file_id`, pages in buffer using that file_id are cleared
    void remove(const FileId file_id);

    // delete the file represented by `tmp_file_id`, pages in private buffer using that tmp_file_id are cleared
    void remove_tmp(const TmpFileId tmp_file_id);

private:
    // folder where all the used files will be
    const std::string db_folder;

    // contains all file streams that have been opened, except for these that were removed
    std::vector< std::unique_ptr<std::fstream> > opened_files;

    std::queue<FileId> available_file_ids;

    std::map<std::string, FileId> filename2file_id;

    uint_fast32_t tmp_filename_counter; // TODO: we may need infinite files

    // to avoid synchronization problems when establishing a new file_id in `get_file_id(filename)`
    std::mutex files_mutex;

    // Contains all filenames that are being used (removed files are not in this list).
    // The position in this vector is equivalent to the FileId representing that file
    std::vector<std::string> filenames;

    // private constructor, other classes must use the global object `file_manager`
    FileManager(const std::string& db_folder);

    // write the data pointed by `bytes` page represented by `page_id` to disk.
    // `bytes` must point to the start memory position of `Page::MDB_PAGE_SIZE` allocated bytes
    void flush(PageId page_id, char* bytes) const;

    // read a page from disk into memory pointed by `bytes`.
    // `bytes` must point to the start memory position of `Page::MDB_PAGE_SIZE` allocated bytes
    void read_page(PageId page_id, char* bytes) const;

    inline const std::string get_file_path(const std::string& filename) const noexcept {
        return db_folder + "/" + filename;
    }
};

extern FileManager& file_manager; // global object

#endif // STORAGE__FILE_MANAGER_H_
