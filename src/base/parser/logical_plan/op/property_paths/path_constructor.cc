#include "path_constructor.h"

#include "base/exceptions.h"
#include "base/parser/logical_plan/op/op_path_alternatives.h"
#include "base/parser/logical_plan/op/op_path_atom.h"
#include "base/parser/logical_plan/op/op_path_sequence.h"
#include "base/parser/logical_plan/op/op_path_kleene_star.h"
#include "base/parser/logical_plan/op/op_path_optional.h"

#include <cassert>
#include <vector>

using namespace std;

unique_ptr<OpPath> PathConstructor::operator()(query::ast::PropertyPathAlternatives& p, bool inverse) {
    assert(p.alternatives.size() > 0);
    if (p.alternatives.size() > 1) {
        vector<unique_ptr<OpPath>> alternatives;

        for (auto& alternative : p.alternatives) {
            alternatives.push_back( (*this)(alternative, inverse) );
        }

        return make_unique<OpPathAlternatives>(move(alternatives));
    } else {
        return (*this)(p.alternatives[0], inverse);
    }
}


unique_ptr<OpPath> PathConstructor::operator()(query::ast::PropertyPathSequence& p, bool inverse) {
    assert(p.atoms.size() > 0);
    if (p.atoms.size() > 1) {
        vector<unique_ptr<OpPath>> sequence;

        if (inverse) {
            for (signed int i = p.atoms.size() - 1; i >= 0; i--) {
                sequence.push_back( (*this)(p.atoms[i], inverse) );
            }
        } else {
            for (auto& atom : p.atoms) {
                sequence.push_back( (*this)(atom, inverse) );
            }
        }

        return make_unique<OpPathSequence>(move(sequence));
    } else {
        return (*this)(p.atoms[0], inverse);
    }
}


unique_ptr<OpPath> PathConstructor::operator()(query::ast::PropertyPathAtom& p, bool inverse) {
    unique_ptr<OpPath> tmp;

    if (p.atom.type() == typeid(std::string)) {
        string s = boost::get<string>(p.atom);
        tmp = make_unique<OpPathAtom>(s, p.inverse ^ inverse); // ^ = XOR
    } else {
        query::ast::PropertyPathAlternatives a = boost::get<query::ast::PropertyPathAlternatives>(p.atom);
        tmp = (*this)(a, p.inverse ^ inverse); // ^ = XOR
    }
    vector<unique_ptr<OpPath>> op_vector;
    if (p.suffix.type() == typeid(query::ast::PropertyPathSuffix)) {
        query::ast::PropertyPathSuffix suffix = boost::get<query::ast::PropertyPathSuffix>(p.suffix);
        switch (suffix) {
            case query::ast::PropertyPathSuffix::NONE :
                return tmp;
            case query::ast::PropertyPathSuffix::ZERO_OR_MORE :
                return make_unique<OpPathKleeneStar>(move(tmp));
            case query::ast::PropertyPathSuffix::ONE_OR_MORE :
                op_vector.push_back(tmp->duplicate());
                op_vector.push_back(make_unique<OpPathKleeneStar>(move(tmp)));
                return make_unique<OpPathSequence>(move(op_vector));
            case query::ast::PropertyPathSuffix::ZERO_OR_ONE :
                return make_unique<OpPathOptional>(move(tmp));
        }
    }
    query::ast::PropertyPathBoundSuffix suffix = boost::get<query::ast::PropertyPathBoundSuffix>(p.suffix);
    if (suffix.min > suffix.max) {
        throw QuerySemanticException("Ill-formed property path. Suffix {m,n} m cannot be greater than n");
    }
    for (uint64_t i = 0; i < suffix.min; i++) {
        op_vector.push_back(tmp->duplicate());
    }
    auto optional = make_unique<OpPathOptional>(tmp->duplicate());

    for (uint64_t i = suffix.min + 1; i < suffix.max; i++) {
        vector<unique_ptr<OpPath>> seq_vector;
        seq_vector.push_back(tmp->duplicate());
        seq_vector.push_back(move(optional));
        optional = make_unique<OpPathOptional>(
            make_unique<OpPathSequence>(move(seq_vector))
        );
    }
    op_vector.push_back(move(optional));
    return make_unique<OpPathSequence>(move(op_vector));
}
