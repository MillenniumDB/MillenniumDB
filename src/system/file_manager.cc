#include "file_manager.h"

#include <fcntl.h>
#include <sys/stat.h>

#include <cassert>
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

void FileManager::flush(VPage& page) const
{
    auto fd = page.page_id.file_id.id;
    lseek(fd, page.page_id.page_number * VPage::SIZE, SEEK_SET);
    auto write_res = write(fd, page.get_bytes(), VPage::SIZE);
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

void FileManager::flush(UPage& page) const
{
    auto fd = page.page_id.file_id.id;
    lseek(fd, page.page_id.page_number * UPage::SIZE, SEEK_SET);
    auto write_res = write(fd, page.get_bytes(), UPage::SIZE);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into str hash file when flushing page");
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

void FileManager::read_existing_page(PageId page_id, char* bytes) const
{
    static_assert((VPage::SIZE == UPage::SIZE), "read_existing_page used for VPage and UPage");

    auto fd = page_id.file_id.id;

#ifndef NDEBUG
    struct stat buf;
    fstat(fd, &buf);
    uint64_t file_size = buf.st_size;
    assert(page_id.page_number < file_size / VPage::SIZE);
#endif

    auto read_res = pread(fd, bytes, VPage::SIZE, page_id.page_number * VPage::SIZE);
    if (read_res == -1) {
        throw std::runtime_error("Could not read file page");
    }
}

uint32_t FileManager::append_page(FileId file_id, char* bytes) const
{
    static_assert((VPage::SIZE == UPage::SIZE), "append_page used for both VPage and UPage");

    auto fd = file_id.id;
    auto page_number = lseek(fd, 0, SEEK_END) / VPage::SIZE;

    // fill the new page with zeros
    memset(bytes, 0, VPage::SIZE);
    auto write_res = write(fd, bytes, VPage::SIZE);

    if (write_res == -1) {
        throw std::runtime_error("Could not write into file");
    }

    return page_number;
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
        return res;
    }
}

void FileManager::init_file(const string& filename) const
{
    const auto file_path = get_file_path(filename);

    auto fd = open(
        file_path.c_str(),
        O_RDWR | O_CREAT,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
    );
    if (fd == -1) {
        throw std::runtime_error("Could not open file " + file_path);
    }

    // fill the new page with zeros
    char* buf = new char[VPage::SIZE];
    memset(buf, 0, VPage::SIZE);
    auto write_res = write(fd, buf, VPage::SIZE);
    delete[] buf;

    if (write_res == -1) {
        throw std::runtime_error("Could not write into file");
    }

    close(fd);
}
