#include "tmp_manager.h"

#include <cassert>

#include "query/exceptions.h"
#include "char_iter.h"
#include "query/query_context.h"

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


void TmpManager::reset() {
    auto idx = get_query_ctx().thread_info.worker_index;
    info[idx].reset();
}

std::unique_ptr<CharIter> TmpManager::get_str_char_iter(uint64_t id) const {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    return std::make_unique<StringTmpIter>(_info.id_to_str[id]);
}


std::unique_ptr<CharIter> TmpManager::get_language_char_iter(uint64_t id) const {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    return std::make_unique<StringTmpIter>(_info.id_to_lan[id]);
}


std::unique_ptr<CharIter> TmpManager::get_datatype_char_iter(uint64_t id) const {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    return std::make_unique<StringTmpIter>(_info.id_to_dtt[id]);
}

void TmpManager::print_str(std::ostream& os, uint64_t id) const {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    if (id >= _info.next_str_id) return; // handle error?
    os << _info.id_to_str[id];
}

void TmpManager::print_dtt(std::ostream& os, uint64_t id) const {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    if (id >= _info.next_dtt_id) return; // handle error?
    os << _info.id_to_dtt[id];
}

void TmpManager::print_lan(std::ostream& os, uint64_t id) const {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    if (id >= _info.next_lan_id) return; // handle error?
    os << _info.id_to_lan[id];
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

uint64_t TmpManager::get_dtt_id(const std::string& dtt) {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    auto id = _info.dtt_to_id.find(dtt);
    if (id != _info.dtt_to_id.end()) {
        return id->second;
    } else {
        _info.dtt_to_id.insert({dtt, _info.next_dtt_id});
        _info.id_to_dtt.push_back(dtt);
        return _info.next_dtt_id++;
    }
}

uint64_t TmpManager::get_lan_id(const std::string& lan) {
    auto idx = get_query_ctx().thread_info.worker_index;
    auto& _info = info[idx];
    auto id = _info.lan_to_id.find(lan);
    if (id != _info.lan_to_id.end()) {
        return id->second;
    } else {
        _info.lan_to_id.insert({lan, _info.next_lan_id});
        _info.id_to_lan.push_back(lan);
        return _info.next_lan_id++;
    }
}
