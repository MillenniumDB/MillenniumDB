#include "buffer_manager.h"

#include <cassert>
#include <type_traits>

#include "macros/aligned_alloc.h"
#include "misc/fatal_error.h"
#include "query/query_context.h"
#include "system/file_manager.h"
#include "system/tensor_manager.h"

// memory for the object
static typename std::aligned_storage<sizeof(BufferManager), alignof(BufferManager)>::type buffer_manager_buf;

// global object
BufferManager& buffer_manager = reinterpret_cast<BufferManager&>(buffer_manager_buf);

BufferManager::BufferManager(
    uint64_t vpage_buffer_pool_size,
    uint64_t ppage_buffer_pool_size_per_worker,
    uint64_t workers
) :
    vp_pool(new Page[vpage_buffer_pool_size]),
    vp_data(reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(vpage_buffer_pool_size * Page::SIZE))),
    vp_pool_size(vpage_buffer_pool_size),
    pp_pool(new PPage[ppage_buffer_pool_size_per_worker * workers]),
    pp_data(
        reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(ppage_buffer_pool_size_per_worker * workers * PPage::SIZE))
    ),
    pp_pool_size(ppage_buffer_pool_size_per_worker)
{
    if (vp_data == nullptr || vp_pool == nullptr) {
        FATAL_ERROR("Could not allocate versioned page buffers, try using a smaller size");
    }

    if (pp_data == nullptr || pp_pool == nullptr) {
        FATAL_ERROR("Could not allocate private page buffers, try using a smaller size");
    }

    for (uint64_t i = 0; i < vp_pool_size; i++) {
        vp_pool[i].set_bytes(&vp_data[i * Page::SIZE]);
    }

    for (uint64_t i = 0; i < pp_pool_size * workers; i++) {
        pp_pool[i].set_bytes(&pp_data[i * PPage::SIZE]);
    }

    pp_map.resize(workers);
    pp_clocks.resize(workers);
    tmp_info.resize(workers);

    vp_map.reserve(vpage_buffer_pool_size);
}

BufferManager::~BufferManager()
{
    while (!pending_writes.empty()) {
        auto page = pending_writes.front();

        assert(page->pins == 0);
        assert(page->dirty);
        if (page->next_version == nullptr) {
            file_manager.flush(*page);
        }
        // page->dirty = false;

        pending_writes.pop();
    }

    delete[] (vp_pool);
    delete[] (pp_pool);
    MDB_ALIGNED_FREE(vp_data);
    MDB_ALIGNED_FREE(pp_data);
}

void BufferManager::init(
    uint64_t versioned_page_buffer_size_in_bytes,
    uint64_t private_page_buffer_pool_size_per_worker_in_bytes,
    uint64_t workers
)
{
    // placement new
    new (&buffer_manager) BufferManager(
        versioned_page_buffer_size_in_bytes / Page::SIZE,
        private_page_buffer_pool_size_per_worker_in_bytes / PPage::SIZE,
        workers
    );
}

// We assume this executes on one thread at a time, controlled by vp_mutex
Page& BufferManager::get_vpage_available()
{
    while (true) {
        vp_clock++;
        vp_clock = vp_clock < vp_pool_size ? vp_clock : 0;

        auto& page = vp_pool[vp_clock];

        if (page.pins != 0 && page.dirty) {
            continue;
        }
        if (page.second_chance) {
            page.second_chance = false;
            continue;
        }
        if (page.prev_version == nullptr && page.next_version == nullptr) {
            if (page.page_id.file_id.id != FileId::UNASSIGNED) {
                vp_map.erase(page.page_id);
            }
            return page;
        }

        running_version_count_mutex.lock();
        auto it = running_version_count.find(page.version_number);
        bool version_not_being_used = it == running_version_count.end();
        running_version_count_mutex.unlock();

        if (version_not_being_used) {
            if (page.prev_version != nullptr) {
                page.prev_version->next_version = page.next_version;
            } else {
                // page is the first in the linked list
                // and we know page.next_version != nullptr (otherwise we would have returned earlier)
                // if there are more versions we need to edit vp_map to point to the new oldest version
                if (page.page_id.file_id.id != FileId::UNASSIGNED) {
                    auto it2 = vp_map.find(page.page_id);
                    assert(it2 != vp_map.end());
                    vp_map.erase(it2);
                    vp_map.insert({ page.page_id, page.next_version });
                }
            }

            if (page.next_version != nullptr) {
                page.next_version->prev_version = page.prev_version;
            }

            return page;
        }
    }
    return vp_pool[vp_clock];
}

PPage& BufferManager::get_ppage_available(uint_fast32_t thread_pos) noexcept
{
    auto& clock = pp_clocks[thread_pos];
    do {
        clock++;
        clock = clock < pp_pool_size ? clock : 0;

        // when pins == 0 the are no synchronization problems with pins and usage
        auto& page = pp_pool[(thread_pos * pp_pool_size) + clock];
        if (page.pins == 0) {
            if (!page.second_chance) {
                return page;
            } else {
                page.second_chance = false;
            }
        }
    } while (true);
}

// use query_context result_version if it exists, otherwise use start_version
Page& BufferManager::get_page_readonly(FileId file_id, uint64_t page_number) noexcept
{
    const PageId page_id(file_id, page_number);

    uint64_t start_version = get_query_ctx().start_version;
    uint64_t result_version = get_query_ctx().result_version;

    vp_mutex.lock();
    auto it = vp_map.find(page_id);

    if (it == vp_map.end()) {
        auto& page = get_vpage_available();

        page.reassign(page_id);
        page.version_number = start_version;
        page.prev_version = nullptr;
        page.next_version = nullptr;
        vp_map.insert({ page_id, &page });

        if (page_id.page_number >= file_manager.count_pages(page_id.file_id)) {
            // appending a new page
            memset(page.bytes, 0, Page::SIZE);
            appended_pages[file_id.id]++;
            current_modifications.push_back(&page);
            page.dirty = true;
            page.version_number = result_version;
        } else {
            file_manager.read_page(page_id, page.bytes);
        }

        vp_mutex.unlock();

        return page;
    } else {
        // page is the buffer, search the corresponding version
        Page* page = it->second;

        while (page->next_version != nullptr && page->next_version->version_number <= result_version) {
            page = page->next_version;
        }

        assert(page->version_number <= result_version);

        page->pin();
        vp_mutex.unlock();

        return *page;
    }
}

bool BufferManager::need_edit_version(const Page& page)
{
    uint64_t result_version = get_query_ctx().result_version;
    return page.version_number != result_version;
}

// use query_context result_version (creating it if not exists)
Page& BufferManager::get_page_editable(FileId file_id, uint64_t page_number) noexcept
{
    const PageId page_id(file_id, page_number);

    uint64_t start_version = get_query_ctx().start_version;
    uint64_t result_version = get_query_ctx().result_version;

    std::lock_guard<std::mutex> lck(vp_mutex);
    auto it = vp_map.find(page_id);

    if (it == vp_map.end()) {
        auto& new_page = get_vpage_available();
        new_page.reassign(page_id); // this will pin the page
        new_page.version_number = result_version;
        new_page.next_version = nullptr;
        new_page.dirty = true;

        if (page_id.page_number < file_manager.count_pages(page_id.file_id)) {
            auto& old_page = get_vpage_available();
            old_page.reassign_page_id(page_id);
            old_page.version_number = start_version;
            old_page.prev_version = nullptr;
            old_page.next_version = &new_page;

            new_page.prev_version = &old_page;

            vp_map.insert({ page_id, &old_page });
            file_manager.read_page(page_id, old_page.bytes);
            std::memcpy(new_page.bytes, old_page.bytes, Page::SIZE);
        } else {
            // appending new page
            vp_map.insert({ page_id, &new_page });
            new_page.prev_version = nullptr;
            std::memset(new_page.bytes, 0, Page::SIZE);
            appended_pages[file_id.id]++;
        }
        current_modifications.push_back(&new_page);

        return new_page;
    } else {
        // page is in the buffer, search the corresponding version
        Page* vpage_head = it->second;

        Page* vpage_tail = vpage_head;
        while (vpage_tail->next_version != nullptr) {
            vpage_tail = vpage_tail->next_version;
        }

        assert(vpage_tail->version_number <= result_version);

        vpage_head->pin();
        vpage_tail->pin();

        if (vpage_tail->version_number != result_version) {
            auto& new_page = get_vpage_available();

            vpage_head->unpin();
            vpage_tail->unpin();

            new_page.reassign(page_id);

            vpage_tail->next_version = &new_page;

            new_page.version_number = result_version;
            new_page.prev_version = vpage_tail;
            new_page.next_version = nullptr;
            new_page.dirty = true;

            current_modifications.push_back(&new_page);

            std::memcpy(new_page.bytes, vpage_tail->bytes, Page::SIZE);
            return new_page;
        } else {
            // vpage_tail is already pinned
            vpage_head->unpin();
            return *vpage_tail;
        }
    }
}

Page& BufferManager::append_page(FileId file_id)
{
    uint64_t result_version = get_query_ctx().result_version;

    std::lock_guard<std::mutex> lck(vp_mutex);

    auto& new_page = get_vpage_available(); // need to have vp_mutex locked

    auto page_number = file_manager.count_pages(file_id) + appended_pages[file_id.id];
    appended_pages[file_id.id]++;

    PageId page_id(file_id, page_number);
    new_page.reassign(page_id);

    new_page.version_number = result_version;
    new_page.prev_version = nullptr;
    new_page.next_version = nullptr;

    std::memset(new_page.bytes, 0, Page::SIZE);

    new_page.dirty = true;

    vp_map.insert({ page_id, &new_page });

    current_modifications.push_back(&new_page);
    return new_page;
}

PPage& BufferManager::get_ppage(TmpFileId tmp_file_id, uint64_t page_number) /*noexcept*/
{
    const TmpPageId tmp_page_id(tmp_file_id.id, page_number);
    const auto worker = get_query_ctx().thread_info.worker_index;

    auto it = pp_map[worker].find(tmp_page_id);
    if (it == pp_map[worker].end()) {
        auto& page = get_ppage_available(worker);
        if (page.page_id.id != TmpPageId::UNASSIGNED_ID) {
            pp_map[worker].erase(page.page_id);

            auto& evicted_info = tmp_info[worker][page.page_id.id];

            // if file does not exists, create it
            if (evicted_info.fd == -1) {
                std::FILE* new_tmp_file = std::tmpfile();
                evicted_info.fd = fileno(new_tmp_file);

                if (evicted_info.fd == -1) {
                    throw std::runtime_error("Could not open tmp file");
                }
            }

            if (page.dirty) {
                // if real size is less than page_number, resize file
                if (evicted_info.real_size <= page.get_page_number()) {
                    auto write_res = ftruncate(evicted_info.fd, PPage::SIZE * (page.get_page_number() + 1));

                    if (write_res == -1) {
                        throw std::runtime_error("Could not truncate tmp file");
                    }
                    evicted_info.real_size = page.get_page_number() + 1;
                }

                file_manager.flush(evicted_info.fd, page);
            }
        }

        page.reassign(tmp_page_id);

        auto& new_info = tmp_info[worker][tmp_file_id.id];

        assert(new_info.logical_size >= new_info.real_size);
        if (page_number < new_info.real_size) {
            // read from disk as this is an existing page
            file_manager.read_tmp_page(new_info.fd, page_number, page.bytes);
        } else {
            // do not read from disk as this is a new page, just assign zeros
            memset(page.bytes, 0, PPage::SIZE);
            new_info.logical_size = page_number + 1;
        }
        pp_map[worker].insert({ tmp_page_id, &page });
        return page;
    } else {
        it->second->pins++;
        return *it->second;
    }
}

TmpFileId BufferManager::get_tmp_file_id()
{
    auto worker = get_query_ctx().thread_info.worker_index;
    auto file_id = tmp_info[worker].size();
    tmp_info[worker].emplace_back();
    return TmpFileId(file_id);
}

void BufferManager::remove_tmp(TmpFileId tmp_file_id)
{
    assert(pp_pool != nullptr);
    auto worker = get_query_ctx().thread_info.worker_index;
    auto offset = pp_pool_size * worker;
    for (uint64_t i = 0; i < pp_pool_size; i++) {
        auto page_id = pp_pool[offset + i].page_id;
        if (page_id.id == tmp_file_id.id) {
            pp_map[worker].erase(page_id);
            pp_pool[offset + i].reset();
        }
    }

    // close file if it was materialized
    auto fd = tmp_info[worker][tmp_file_id.id].fd;
    if (fd != -1) {
        tmp_info[worker][tmp_file_id.id].fd = -1;
        close(fd);
    }
}

BufferManager::VersionScope::VersionScope(uint64_t start_version, bool is_editable) :
    start_version(start_version),
    string_manager_original_end(string_manager.get_end()),
    tensor_manager_original_end(tensor_manager.get_end()),
    is_editable(is_editable)
{ }

BufferManager::VersionScope::~VersionScope()
{
    tmp_manager.reset_tmp_list();
    buffer_manager.terminate(*this);
}

std::unique_ptr<BufferManager::VersionScope> BufferManager::init_version_readonly()
{
    std::lock_guard<std::mutex> lck(running_version_count_mutex);
    auto ver = last_stable_version;
    running_version_count[ver]++;
    auto worker = get_query_ctx().thread_info.worker_index;
    tmp_info[worker].clear();
    return std::make_unique<BufferManager::VersionScope>(ver, false);
}

std::unique_ptr<BufferManager::VersionScope> BufferManager::init_version_editable()
{
    std::lock_guard<std::mutex> lck(running_version_count_mutex);
    auto ver = last_stable_version;
    running_version_count[ver]++;
    running_version_count[ver + 1]++;
    auto worker = get_query_ctx().thread_info.worker_index;
    tmp_info[worker].clear();
    return std::make_unique<BufferManager::VersionScope>(ver, true);
}

bool BufferManager::version_not_being_used(uint64_t version_number)
{
    running_version_count_mutex.lock();
    const auto it = running_version_count.find(version_number);
    const bool version_not_used = it == running_version_count.end();
    running_version_count_mutex.unlock();
    return version_not_used;
}

bool BufferManager::is_editable(Page& page) const
{
    const uint64_t start_version = get_query_ctx().start_version;
    const uint64_t result_version = get_query_ctx().result_version;
    return page.version_number == result_version && result_version > start_version;
}

void BufferManager::terminate(const VersionScope& version_scope)
{
    std::lock_guard<std::mutex> lck(running_version_count_mutex);
    auto it1 = running_version_count.find(version_scope.start_version);
    assert(it1 != running_version_count.end());

    it1->second--;

    if (it1->second == 0) {
        running_version_count.erase(it1);
    }
    if (version_scope.is_editable) {
        auto it2 = running_version_count.find(version_scope.start_version + 1);
        assert(it2 != running_version_count.end());

        it2->second--;

        if (it2->second == 0) {
            running_version_count.erase(it2);
        }

        last_stable_version++;

        if (version_scope.commited) {
            std::lock_guard<std::mutex> pending_write_mutex_lck(pending_write_mutex);
            for (Page* page : current_modifications) {
                pending_writes.push(page);
            }
            file_manager.update_appends(appended_pages);
        } else {
            string_manager.rollback(version_scope.string_manager_original_end);
            tensor_manager.rollback(version_scope.tensor_manager_original_end);
            for (Page* page : current_modifications) {
                if (auto prev_page = page->prev_version) {
                    prev_page->next_version = nullptr;
                }
                page->reset();
            }
        }
        appended_pages.clear();
        current_modifications.clear();
    }
}

bool BufferManager::try_flush_page()
{
    std::lock_guard<std::mutex> pending_write_mutex_lck(pending_write_mutex);
    if (pending_writes.empty()) {
        return false;
    }

    auto page = pending_writes.front();

    assert(page->pins == 0);
    assert(page->dirty);
    if (page->next_version == nullptr) {
        file_manager.flush(*page);
    }
    page->dirty = false;

    pending_writes.pop();
    return true;
}
