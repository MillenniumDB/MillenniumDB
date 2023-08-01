#include "join_1_var.h"

#include <cassert>

#include "storage/page.h"

using namespace std;
using namespace HashJoin;
using namespace HashJoin::BGP;
using namespace HashJoin::BGP::InMemory;

Join1Var::Join1Var(
    unique_ptr<BindingIter>  _build_rel,
    unique_ptr<BindingIter>  _probe_rel,
    vector<VarId>&&            _join_vars,
    vector<VarId>&&            _build_vars,
    vector<VarId>&&            _probe_vars
) :
    probe_rel         (std::move(_probe_rel)),
    build_rel         (std::move(_build_rel)),
    join_vars         (std::move(_join_vars)),
    build_vars        (std::move(_build_vars)),
    probe_vars        (std::move(_probe_vars))
{
    assert(join_vars.size() == 1);

    data_chunk = new uint64_t[(build_vars.size() + 1) * Page::MDB_PAGE_SIZE];
    data_chunks_dir.push_back(data_chunk);
    data_chunk_index = 0;
}


Join1Var::~Join1Var() {
    // Avoid mem leaks
    for (auto block : data_chunks_dir) {
        delete[](block);
    }
}


void Join1Var::begin(Binding& _parent_binding) {
    // set hash join in start state, always must be non enumerating_row
    enumerating_rows = nullptr;

    this->parent_binding = &_parent_binding;
    build_rel->begin(_parent_binding);
    probe_rel->begin(_parent_binding);

    build_hash_table();
}


bool Join1Var::next() {
    while (true) {
        if (enumerating_rows != nullptr) {
            for (uint_fast32_t i = 0; i < build_vars.size(); i++) {
                parent_binding->add(build_vars[i], ObjectId(enumerating_rows[i]));
            }

            enumerating_rows = reinterpret_cast<uint64_t**>(enumerating_rows)[build_vars.size()];
            found++;
            return true;
        }
        else {
            if (probe_rel->next()) {
                auto iterator = hash_table.find((*parent_binding)[join_vars[0]]);
                if (iterator != hash_table.end()) {
                    enumerating_rows = (iterator->second).head;
                }
            } else {
                return false;
            }
        }
    }
}


void Join1Var::reset() {
    hash_table.clear();

    // Delete chunks except first to avoid an unnecessary
    // request for space
    for (size_t i = 1; i < data_chunks_dir.size(); i++) {
        delete[] (data_chunks_dir[i]);
    }

    // Resize directories
    data_chunks_dir.resize(1);
    data_chunk_index = 0;
    data_chunk = data_chunks_dir[0];

    // Set enumerating rows as nullptr like in begin
    enumerating_rows = nullptr;

    // Spread reset to children
    build_rel->reset();
    probe_rel->reset();

    build_hash_table();
}


void Join1Var::assign_nulls() {
    build_rel->assign_nulls();
    probe_rel->assign_nulls();
}


void Join1Var::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Join1Var(found: " << found << "\n";
    build_rel->analyze(os, indent + 2);
    os << ",\n";
    probe_rel->analyze(os, indent + 2);
    os << "\n";
    os << std::string(indent, ' ');
    os << ")";
}


void Join1Var::build_hash_table() {
    auto data_tuple_size = build_vars.size() + 1;

    while (build_rel->next()) {
        auto start_data_index = data_chunk_index * data_tuple_size;
        // Store data
        for (size_t i = 0; i < build_vars.size(); i++) {
            data_chunk[start_data_index + i] =  (*parent_binding)[build_vars[i]].id;
        }
        // Set last data value as a null pointer
        auto casted_chunk = reinterpret_cast<uint64_t**>(data_chunk);
        casted_chunk[start_data_index + build_vars.size()] = nullptr;

        // Store pointer to the begin of the new row of data added
        auto data_pointer = &(data_chunk[start_data_index]);

        // Check if data chunk is full and add a new one if is needed
        data_chunk_index++;
        if (data_chunk_index == Page::MDB_PAGE_SIZE) {
            data_chunk = new uint64_t[data_tuple_size * Page::MDB_PAGE_SIZE];
            data_chunks_dir.push_back(data_chunk);
            data_chunk_index = 0;
        }

        auto iterator = hash_table.emplace((*parent_binding)[join_vars[0]],
                                            Value(data_pointer,
                                                    data_pointer)
                                );
        if (!(iterator.second)) {
            auto casted_tail = reinterpret_cast<uint64_t**>(iterator.first->second.tail);
            casted_tail[build_vars.size()] = data_pointer;
            iterator.first->second.tail = data_pointer;
        }
    }
}
