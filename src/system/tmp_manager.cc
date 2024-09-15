#include "tmp_manager.h"

#include <cassert>

#include "query/query_context.h"
#include "storage/char_iter.h"

// memory for the object
static typename std::aligned_storage<sizeof(TmpManager), alignof(TmpManager)>::type temporal_manager_buf;
// global object
TmpManager& tmp_manager = reinterpret_cast<TmpManager&>(temporal_manager_buf);


void TmpManager::init(int num_workers) {
    new (&tmp_manager) TmpManager(num_workers); // placement new
}


TmpManager::TmpManager(int num_workers) {
    for (int i = 0; i < num_workers; i++) {
        info.emplace_back();
    }
}


TmpManager::~TmpManager() { }


void TmpManager::reset(uint_fast32_t worker_idx) {
    info[worker_idx].reset();
}


std::unique_ptr<CharIter> TmpManager::get_str_char_iter(uint64_t id) const {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    return std::make_unique<StringTmpIter>(_info.id_to_str[id]);
}


const std::string& TmpManager::get_str(uint64_t id) const {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    // if (id >= _info.next_str_id) return; // handle error?
    return _info.id_to_str[id];
}


void TmpManager::print_str(std::ostream& os, uint64_t id) const {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    if (id >= _info.next_str_id) return; // handle error?
    os << _info.id_to_str[id];
}


uint64_t TmpManager::get_str_id(const std::string& str) {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    auto id = _info.str_to_id.find(str);
    if (id != _info.str_to_id.end()) {
        return id->second;
    } else {
        _info.str_to_id.insert({str, _info.next_str_id});
        _info.id_to_str.push_back(str);
        return _info.next_str_id++;
    }
}
