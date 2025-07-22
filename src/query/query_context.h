#pragma once

#include <cassert>
#include <chrono>
#include <ostream>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "graph_models/object_id.h"
#include "query/id.h"
#include "query/var_id.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

struct ThreadInfo {
    bool interruption_requested = false;

    uint_fast32_t worker_index = 0;

    std::chrono::system_clock::time_point timeout;
    std::chrono::system_clock::time_point time_start;
};


class QueryContext {
struct VarContext {
    uint64_t internal_var_counter = 0;

    std::vector<std::string> var_names;

    // maps var_name -> pos
    std::unordered_map<std::string, uint64_t> var_map;
};

public:
    ThreadInfo thread_info;

    uint64_t start_version = 0;
    uint64_t result_version = 0;

    std::string cancellation_token;

    // Used only by BindingExprBNode of the RDF model.
    std::unordered_map<std::string, uint64_t> blank_node_ids;

    // Debug prints ObjectIds
    static inline std::ostream& (*_debug_print)(std::ostream&, ObjectId);

    static constexpr uint64_t buffer_size = 64 * 1024;

private:
    VarContext var_ctx;

    // Used only by BindingExprBNode of the RDF model.
    uint64_t blank_node_count = 0;

    std::default_random_engine rand_generator = std::default_random_engine(
        std::chrono::system_clock::now().time_since_epoch().count()
    );

    // Random number generator
    std::uniform_real_distribution<double> distribution = std::uniform_real_distribution<double>(0.0, 1.0);

    static inline boost::uuids::random_generator uuid_generator;

    // buffers for general use
    char* buffer1;
    char* buffer2;

    std::map<VarId, ObjectId> edge_directions;

public:
    QueryContext()
    {
        buffer1 = new char[StringManager::MAX_STRING_SIZE];
        buffer2 = new char[StringManager::MAX_STRING_SIZE];
    }

    QueryContext(QueryContext&& other) :
        buffer1(std::exchange(other.buffer1, nullptr)),
        buffer2(std::exchange(other.buffer2, nullptr))
    { }

    QueryContext(const QueryContext& other) = delete;

    ~QueryContext()
    {
        delete[] buffer1;
        delete[] buffer2;
    }

    // Cleans up everything. Must be called before parsing the query
    void prepare(BufferManager::VersionScope& version_scope, std::chrono::seconds timeout) {
        blank_node_ids.clear();
        blank_node_count = 0;

        var_ctx.internal_var_counter = 0;
        var_ctx.var_names = {};
        var_ctx.var_map.clear();

        const auto start = std::chrono::system_clock::now();
        thread_info.interruption_requested = false;
        thread_info.time_start = start;
        thread_info.timeout = start + timeout;

        start_version  = version_scope.start_version;
        result_version = version_scope.start_version + (version_scope.is_editable ? 1 : 0);

        cancellation_token = get_uuid();

        tmp_manager.reset(thread_info.worker_index);
    }

    std::string get_uuid() {
        boost::uuids::uuid uuid = uuid_generator();
        return boost::uuids::to_string(uuid);
    }

    // returns a random double between 0 (inclusive) and 1 (exclusive) with uniform distribution
    double get_rand() {
        return distribution(rand_generator);
    }


    VarId get_or_create_var(const std::string& var_name) {
        auto found = var_ctx.var_map.find(var_name);
        if (found != var_ctx.var_map.end()) {
            return VarId(found->second);
        } else {
            uint64_t new_id = var_ctx.var_names.size();
            var_ctx.var_names.push_back(var_name);
            var_ctx.var_map.insert({ var_name, new_id });
            return VarId(new_id);
        }
    }

    VarId get_var(const std::string& var_name, bool* found) {
        auto it = var_ctx.var_map.find(var_name);
        if (it != var_ctx.var_map.end()) {
            *found = true;
            return VarId(it->second);
        } else {
           *found = false;
           return VarId(0);
        }
    }

    const std::string& get_var_name(VarId var_id) {
        assert(var_ctx.var_names.size() > var_id.id);
        return var_ctx.var_names[var_id.id];
    }

    uint64_t get_var_size() const {
        return var_ctx.var_names.size();
    }

    // generates a new internal var.
    VarId get_internal_var() {
        uint64_t new_id = var_ctx.var_names.size();
        var_ctx.var_names.push_back("." + std::to_string(var_ctx.internal_var_counter++));
        return VarId(new_id);
    }

    std::set<VarId> get_all_vars() {
        std::set<VarId> res;
        for (unsigned i = 0; i < var_ctx.var_names.size(); i++) {
            res.emplace(i);
        }
        return res;
    }

    std::set<VarId> get_non_internal_vars() {
        std::set<VarId> res;
        for (unsigned i = 0; i < var_ctx.var_names.size(); i++) {
            if (var_ctx.var_names[i][0] != '.') {
                res.emplace(i);
            }
        }
        return res;
    }

    // only for RDF model
    uint64_t get_new_blank_node() {
        return blank_node_count++;
    }

    void debug_print(std::ostream& os, ObjectId oid) {
        _debug_print(os, oid);
    }

    void debug_print(std::ostream& os, Id id) {
        if (id.is_OID()) {
            _debug_print(os, id.get_OID());
        } else {
            os << get_var_name(id.get_var());
        }
    }

    bool is_internal(VarId var) {
        return var_ctx.var_names[var.id][0] == '.';
    }

    bool is_blank(VarId var) {
        return var_ctx.var_names[var.id].find("_:") == 0;
    }

    static inline thread_local QueryContext* _query_ctx;

    static inline void set_query_ctx(QueryContext* ctx) {
        _query_ctx = ctx;
    }

    char* get_buffer1() {
        return buffer1;
    }

    char* get_buffer2() {
        return buffer2;
    }
};

inline QueryContext& get_query_ctx() {
    return *QueryContext::_query_ctx;
}

inline std::ostream& operator<<(std::ostream& os, ObjectId oid) {
    return get_query_ctx()._debug_print(os, oid);
}

inline std::ostream& operator<<(std::ostream& os, VarId var) {
    return os << '?' << get_query_ctx().get_var_name(var);
}

inline std::ostream& operator<<(std::ostream& os, Id id) {
    if (id.is_OID()) {
        get_query_ctx()._debug_print(os, id.get_OID());
    } else {
        os << '?' << get_query_ctx().get_var_name(id.get_var());
    }
    return os;
}
