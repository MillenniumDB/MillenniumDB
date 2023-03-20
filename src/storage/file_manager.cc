#include "file_manager.h"

#include <sys/stat.h>
#include <fcntl.h>

#include <cassert>
#include <cstring>
#include <new>         // placement new
#include <type_traits> // aligned_storage

#include "storage/buffer_manager.h"
#include "storage/file_id.h"
#include "storage/filesystem.h"
#include "storage/page.h"

using namespace std;

// memory for the object
static typename std::aligned_storage<sizeof(FileManager), alignof(FileManager)>::type file_manager_buf;
// global object
FileManager& file_manager = reinterpret_cast<FileManager&>(file_manager_buf);


FileManager::FileManager(const std::string& db_folder) :
    db_folder(db_folder)
{
    if (Filesystem::exists(db_folder)) {
        if (!Filesystem::is_directory(db_folder)) {
            throw std::invalid_argument("Cannot create database directory: \"" + db_folder +
                                        "\", a file with that name already exists.");
        }
    } else {
        Filesystem::create_directories(db_folder);
    }
}


void FileManager::init(const std::string& db_folder) {
    new (&file_manager) FileManager(db_folder); // placement new
}


void FileManager::flush(Page& page) const {
    auto fd = page.page_id.file_id.id;
    lseek(fd, page.page_id.page_number*Page::MDB_PAGE_SIZE, SEEK_SET);
    auto write_res = write(fd, page.get_bytes(), Page::MDB_PAGE_SIZE);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into file when flushing page");
    }
    page.dirty = false;
}


void FileManager::read_page(PageId page_id, char* bytes) const {
    auto fd = page_id.file_id.id;
    lseek(fd, 0, SEEK_END);

    struct stat buf;
    fstat(fd, &buf);
    uint64_t file_size = buf.st_size;

    lseek(fd, page_id.page_number*Page::MDB_PAGE_SIZE, SEEK_SET);
    if (file_size/Page::MDB_PAGE_SIZE <= page_id.page_number) {
        // new file page, write zeros
        memset(bytes, 0, Page::MDB_PAGE_SIZE);
        auto write_res = ftruncate(fd, Page::MDB_PAGE_SIZE*page_id.page_number);

        if (write_res == -1) {
            throw std::runtime_error("Could not write into file");
        }
    } else {
        // reading existing file page
        auto read_res = read(fd, bytes, Page::MDB_PAGE_SIZE);
        if (read_res == -1) {
            throw std::runtime_error("Could not read file page");
        }
    }
}


FileId FileManager::get_file_id(const string& filename) {
    auto search = filename2file_id.find(filename);
    if (search != filename2file_id.end()) {
        return search->second;
    } else {
        const auto file_path = get_file_path(filename);

        auto fd = open(file_path.c_str(), O_RDWR/*|O_DIRECT*/|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
        if (fd == -1) {
            throw std::runtime_error("Could not open file " + file_path);
        }
        const auto res = FileId(fd);
        filename2file_id.insert({ filename, res });
        return res;
    }
}


TmpFileId FileManager::get_tmp_file_id() {
    std::FILE* tmpf = std::tmpfile();
    auto fd = fileno(tmpf);
    if (fd == -1) {
        throw std::runtime_error("Could not open temp file");
    }
    const auto file_id = FileId(fd);
    return TmpFileId(buffer_manager.get_private_buffer_index(), file_id);
}


void FileManager::remove_tmp(const TmpFileId tmp_file_id) {
    buffer_manager.remove_tmp(tmp_file_id); // clear pages from buffer_manager
    close(tmp_file_id.file_id.id);          // close the file stream, file will be removed
}
