#include <iostream>

#include "query/variable_set.h"

typedef bool TestFunction();

typedef std::vector<decltype(VarId::id)> VarVec;

std::vector<VarId> to_varid(const VarVec& var_vec) {
    std::vector<VarId> varid_vec;
    varid_vec.reserve(var_vec.size());
    for (auto var : var_vec) {
        varid_vec.push_back(VarId(var));
    }
    return varid_vec;
}


VarVec get_variables(const VariableSet& vs) {
    VarVec variables;
    for (auto var : vs) {
        variables.push_back(var.id);
    }
    return variables;
}


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector) {
    os << '{';
    auto first = true;
    for (auto& element : vector) {
        if (first) {
            first = false;
        } else {
            os << ", ";
        }
        os << element;
    }
    os << '}';
    return os;
}


bool insertion_and_iteration() {
    std::vector<std::pair<VarVec, VarVec>> tests = {
        { {}, {} },
        { { 1 }, { 1 } },
        { { 300 }, { 300 } },
        { { 1, 1, 1 }, { 1 } },
        { { 1, 2, 3 }, { 1, 2, 3 } },
        { { 1, 2, 3, 500, 500 }, { 1, 2, 3, 500 } },
        { { 500, 3, 2, 1, 500, 500 }, { 1, 2, 3, 500 } },
        { { 1009, 1001, 1003 }, { 1001, 1003, 1009 } },
    };

    auto error = false;

    for (auto& [var_vec, expected] : tests) {
        VariableSet vs(to_varid(var_vec));

        auto received = get_variables(vs);
        if (received != expected) {
            error = true;
            std::cerr << "Insert " << var_vec << ", received " << received << ", expected " << expected << "\n";
        }
    }

    return error;
}

bool union_() {
    std::vector<std::pair<std::vector<VarVec>, VarVec>> tests = {
        { { {} }, {} },
        { { { 1 } }, { 1 } },
        { { { 1000 } }, { 1000 } },
        { { { 1 }, { 1 } }, { 1 } },
        { { { 1000 }, { 1000 } }, { 1000 } },
        { { { 1 }, { 1000 } }, { 1, 1000 } },
        { { { 1000 }, { 1 } }, { 1, 1000 } },
        { { { 1000, 1 }, { 1, 1000 } }, { 1, 1000 } },
        { { { 1000, 1, 10, 100 }, { 2, 200, 20, 2000 } }, { 1, 2, 10, 20, 100, 200, 1000, 2000 } },
        { { { 1000, 1 }, { 2, 2000 }, { 10 }, { 5000 } }, { 1, 2, 10, 1000, 2000, 5000 } },
    };

    auto error = false;

    for (auto& [var_vecs, expected] : tests) {
        VariableSet res(to_varid(var_vecs[0]));

        for (size_t i = 1; i < var_vecs.size(); i++) {
            VariableSet vs(to_varid(var_vecs[i]));
            res = res.union_(vs);
        }

        auto received = get_variables(res);
        if (received != expected) {
            error = true;
            std::cerr << "Union " << var_vecs << ", received " << received << ", expected " << expected << "\n";
        }
    }

    return error;
}


bool intersection() {
    std::vector<std::pair<std::vector<VarVec>, VarVec>> tests = {
        { { {} }, {} },
        { { { 1 } }, { 1 } },
        { { { 1000 } }, { 1000 } },
        { { { 1 }, { 1 } }, { 1 } },
        { { { 1000 }, { 1000 } }, { 1000 } },
        { { { 1 }, { 1000 } }, {} },
        { { { 1000 }, { 1 } }, {} },
        { { { 1000, 1 }, { 1, 1000 } }, { 1, 1000 } },
        { { { 1000, 1, 10, 100 }, { 2, 200, 20, 2000 } }, {} },
        { { { 1000, 1 }, { 2, 1000 }, { 1000 }, { 1000, 5000 } }, { 1000 } },
        { { { 1000, 1 }, { 2, 1000 }, { 1000 }, { 1000, 5000 }, { 1, 2, 5000 } }, {} },
    };

    auto error = false;

    for (auto& [var_vecs, expected] : tests) {
        VariableSet res(to_varid(var_vecs[0]));

        for (size_t i = 1; i < var_vecs.size(); i++) {
            VariableSet vs(to_varid(var_vecs[i]));
            res = res.intersection(vs);
        }

        auto received = get_variables(res);
        if (received != expected) {
            error = true;
            std::cerr << "Intersection " << var_vecs << ", received " << received << ", expected " << expected << "\n";
        }
    }

    return error;
}


bool difference() {
    std::vector<std::pair<std::vector<VarVec>, VarVec>> tests = {
        { { {} }, {} },
        { { { 1 } }, { 1 } },
        { { { 1000 } }, { 1000 } },
        { { { 1 }, { 1 } }, {} },
        { { { 1000 }, { 1000 } }, {} },
        { { { 1 }, { 1000 } }, { 1 } },
        { { { 1000 }, { 1 } }, { 1000 } },
        { { { 1000, 1 }, { 1, 1000 } }, {} },
        { { { 1000, 1, 10, 100 }, { 2, 200, 20, 2000 } }, { 1, 10, 100, 1000 } },
        { { { 1000, 1 }, { 2, 1000 }, { 1000 }, { 1000, 5000 } }, { 1 } },
        { { { 1000, 1 }, { 2, 1000 }, { 1000 }, { 1000, 5000 }, {} }, { 1 } },
        { { { 10, 1, 1000, 100 }, { 2, 1000 }, { 1000, 1 }, { 1000, 5000 }, {} }, { 10, 100 } },
    };

    auto error = false;

    for (auto& [var_vecs, expected] : tests) {
        VariableSet res(to_varid(var_vecs[0]));

        for (size_t i = 1; i < var_vecs.size(); i++) {
            VariableSet vs(to_varid(var_vecs[i]));
            res = res.difference(vs);
        }

        auto received = get_variables(res);
        if (received != expected) {
            error = true;
            std::cerr << "Difference " << var_vecs << ", received " << received << ", expected " << expected << "\n";
        }
    }

    return error;
}


int main() {
    std::vector<TestFunction*> tests;

    tests.push_back(&insertion_and_iteration);
    tests.push_back(&union_);
    tests.push_back(&intersection);
    tests.push_back(&difference);

    auto error = false;

    for (auto& test_func : tests) {
        if (test_func()) {
            error = true;
        }
    }

    return error;
}
