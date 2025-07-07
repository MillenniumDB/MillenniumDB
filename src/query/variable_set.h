#include <cstdint>
#include <ostream>
#include <vector>

#include "query/var_id.h"

class VariableSetIterator;


// ----------------------------- VariableSet -----------------------------


class VariableSet {
    friend VariableSetIterator;

private:
    std::vector<uint64_t> variables;

public:
    VariableSet() { }
    VariableSet(size_t vec_size) : variables(vec_size, 0) { }
    VariableSet(const std::vector<VarId>& vars);

    void insert(VarId var_id);

    VariableSet intersection(const VariableSet& other) const;
    VariableSet union_(const VariableSet& other) const;
    VariableSet difference(const VariableSet& other) const;

    VariableSetIterator begin() const;
    VariableSetIterator end() const;

    void print(std::ostream& os) const;
};


// ----------------------------- Iterator -----------------------------


class VariableSetIterator {
    friend VariableSet;

private:
    const VariableSet& variable_set;

    // index of the current uint64_t in variable_set.variables
    uint64_t vec_idx = 0;

    // copy of variable_set.variables[vec_idx] so we can modify it
    uint64_t current = 0;

    // index of the current bit in current
    uint64_t current_idx = 0;

    // start/end is used to select the constructor at compile time
    struct start { };
    struct end { };
    VariableSetIterator(const VariableSet& vs, start);
    VariableSetIterator(const VariableSet& vs, end);

    // Advances the iterator to the next variable present in the set.
    // If there are no next variables it stops at the end.
    void advance();

    // Checks if the current variable of the iterator is in the set
    bool present();


public:
    bool operator!=(VariableSetIterator rhs);

    VarId operator*();

    VariableSetIterator& operator++();
};

static_assert(std::is_move_assignable<VariableSet>());
static_assert(std::is_move_constructible<VariableSet>());

static_assert(std::is_copy_assignable<VariableSet>());
static_assert(std::is_copy_constructible<VariableSet>());
