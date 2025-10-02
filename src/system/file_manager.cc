#include "file_manager.h"

#include <fcntl.h>
#include <sys/stat.h>

#include <cstring>
#include <type_traits>

#include "storage/file_id.h"
#include "storage/filesystem.h"

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
            throw std::invalid_argument(
                "Cannot create database directory: \"" + db_folder
                + "\", a file with that name already exists."
            );
        }
    } else {
        Filesystem::create_directories(db_folder);
    }
}

void FileManager::init(const std::string& db_folder)
{
    new (&file_manager) FileManager(db_folder); // placement new
}

void FileManager::flush(Page& page) const
{
    auto fd = page.page_id.file_id.id;
    lseek(fd, page.page_id.page_number * Page::SIZE, SEEK_SET);
    auto write_res = write(fd, page.get_bytes(), Page::SIZE);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into file when flushing page");
    }
    page.dirty = false;
}

void FileManager::flush(int fd, PPage& page) const
{
    auto write_res = pwrite(fd, page.get_bytes(), PPage::SIZE, page.page_id.page_number * PPage::SIZE);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into tmp file when flushing page");
    }
    page.dirty = false;
}

void FileManager::read_tmp_page(int fd, uint64_t page_number, char* bytes) const
{
    // reading existing file page
    auto read_res = pread(fd, bytes, PPage::SIZE, page_number * PPage::SIZE);
    if (read_res == -1) {
        throw std::runtime_error("Could not read file page");
    }
}

void FileManager::read_page(PageId page_id, char* bytes) const
{
    auto fd = page_id.file_id.id;

    auto read_res = pread(fd, bytes, Page::SIZE, page_id.page_number * Page::SIZE);
    if (read_res != Page::SIZE) {
        if (fid2pages.find(fd) == fid2pages.end()) {
            memset(bytes, 0, Page::SIZE);
            return;
        }

        for (auto&& [filename, file_id] : filename2file_id) {
            if (page_id.file_id == file_id) {
                auto msg = "Could not read file " + filename + " at page "
                         + std::to_string(page_id.page_number);
                throw std::runtime_error(msg);
            }
        }
        throw std::runtime_error("unrecognized file id");
    }
}

uint32_t FileManager::count_pages(FileId file_id) const
{
    auto it = fid2pages.find(file_id);
    return it == fid2pages.end() ? 0 : it->second;
}

void FileManager::update_appends(const std::map<FileId, unsigned>& appended_pages)
{
    for (auto&& [k, v] : appended_pages) {
        fid2pages[k] += v;
    }
}

FileId FileManager::get_file_id(const string& filename)
{
    auto search = filename2file_id.find(filename);
    if (search != filename2file_id.end()) {
        return search->second;
    } else {
        const auto file_path = get_file_path(filename);

        auto fd = open(file_path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if (fd == -1) {
            throw std::runtime_error("Could not open file " + file_path);
        }
        const auto res = FileId(fd);
        filename2file_id.insert({ filename, res });

        uint32_t page_count = lseek(fd, 0, SEEK_END) / Page::SIZE;
        fid2pages.insert({ res, page_count });

        return res;
    }
}
