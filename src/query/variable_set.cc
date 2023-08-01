#include "variable_set.h"

#include <cassert>

#include "query/query_context.h"

// ----------------------------- VariableSet -----------------------------


VariableSet::VariableSet(const std::vector<VarId>& vars) {
    for (auto var : vars) {
        insert(var);
    }
}


void VariableSet::insert(VarId var_id) {
    auto id = static_cast<uint64_t>(var_id.id);

    while (id >= variables.size() * 64) {
        variables.push_back(0);
    }

    auto vec_idx = id / 64;
    auto int_idx = id % 64;

    variables[vec_idx] |= 1ULL << int_idx;
}


VariableSet VariableSet::intersection(const VariableSet& other) const {
    auto min_size = std::min(variables.size(), other.variables.size());

    VariableSet res(min_size);

    for (size_t i = 0; i < min_size; i++) {
        res.variables[i] = variables[i] & other.variables[i];
    }

    return res;
}


VariableSet VariableSet::union_(const VariableSet& other) const {
    auto min_size = std::min(variables.size(), other.variables.size());
    auto max_size = std::max(variables.size(), other.variables.size());

    VariableSet res(max_size);

    for (size_t i = 0; i < min_size; i++) {
        res.variables[i] = variables[i] | other.variables[i];
    }

    for (size_t i = min_size; i < variables.size(); i++) {
        res.variables[i] = variables[i];
    }

    for (size_t i = min_size; i < other.variables.size(); i++) {
        res.variables[i] = other.variables[i];
    }

    return res;
}


VariableSet VariableSet::difference(const VariableSet& other) const {
    auto min_size = std::min(variables.size(), other.variables.size());

    VariableSet res(*this);

    for (size_t i = 0; i < min_size; i++) {
        res.variables[i] = variables[i] & (~other.variables[i]);
    }

    return res;
}


VariableSetIterator VariableSet::begin() const {
    return VariableSetIterator(*this, VariableSetIterator::start {});
}


VariableSetIterator VariableSet::end() const {
    return VariableSetIterator(*this, VariableSetIterator::end {});
}


void VariableSet::print(std::ostream& os) const {
    os << '[';
    for (auto var : *this) {
        os << ' ' << var.id << "=?" << get_query_ctx().get_var_name(var);
    }
    os << " ]";
}


// ----------------------------- Iterator -----------------------------


VariableSetIterator::VariableSetIterator(const VariableSet& vs, start) : variable_set(vs) {
    if (vs.variables.size() == 0) {
        return;
    }

    current = vs.variables[0];

    if (not present()) {
        advance();
    }
}


VariableSetIterator::VariableSetIterator(const VariableSet& vs, end) : variable_set(vs) {
    vec_idx = vs.variables.size();
}


void VariableSetIterator::advance() {
    current >>= 1;
    current_idx++;

    while (current == 0) {
        vec_idx++;
        current_idx = 0;

        if (vec_idx == variable_set.variables.size()) {
            return;
        }

        current = variable_set.variables[vec_idx];
    }

    auto tz = __builtin_ctzll(current);
    current >>= tz;
    current_idx += tz;
}


bool VariableSetIterator::present() {
    return (current & 1) != 0;
}


bool VariableSetIterator::operator!=(VariableSetIterator rhs) {
    return current_idx != rhs.current_idx or vec_idx != rhs.vec_idx;
}


VarId VariableSetIterator::operator*() {
    assert(present());

    return VarId(vec_idx * 64 + current_idx);
}


VariableSetIterator& VariableSetIterator::operator++() {
    advance();
    return *this;
}