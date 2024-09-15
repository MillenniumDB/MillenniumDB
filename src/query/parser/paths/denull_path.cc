#include "denull_path.h"

#include <cassert>

#include "query/parser/paths/path_alternatives.h"
#include "query/parser/paths/path_atom.h"
#include "query/parser/paths/path_check.h"
#include "query/parser/paths/path_kleene_plus.h"
#include "query/parser/paths/path_kleene_star.h"
#include "query/parser/paths/path_negated_set.h"
#include "query/parser/paths/path_optional.h"
#include "query/parser/paths/path_sequence.h"
#include "query/parser/paths/regular_path_expr.h"

std::unique_ptr<RegularPathExpr> PathDenull::denull(std::unique_ptr<RegularPathExpr> path) {
    auto res = accept_denull(std::move(path));
    assert(res != nullptr);
    return res;
}


std::unique_ptr<RegularPathExpr> PathDenull::accept_denull(std::unique_ptr<RegularPathExpr> path) {
    auto type = path->type();

    switch (type) {
    case PathType::PATH_ALTERNATIVES: {
        std::unique_ptr<PathAlternatives> casted;
        casted.reset(dynamic_cast<PathAlternatives*>(path.release()));
        return denull(std::move(casted));
    }

    case PathType::PATH_SEQUENCE: {
        std::unique_ptr<PathSequence> casted;
        casted.reset(dynamic_cast<PathSequence*>(path.release()));
        return denull(std::move(casted));
    }

    case PathType::PATH_KLEENE_STAR: {
        std::unique_ptr<PathKleeneStar> casted;
        casted.reset(dynamic_cast<PathKleeneStar*>(path.release()));
        return denull(std::move(casted));
    }

    case PathType::PATH_KLEENE_PLUS: {
        std::unique_ptr<PathKleenePlus> casted;
        casted.reset(dynamic_cast<PathKleenePlus*>(path.release()));
        return denull(std::move(casted));
    }

    case PathType::PATH_ATOM: {
        std::unique_ptr<PathAtom> casted;
        casted.reset(dynamic_cast<PathAtom*>(path.release()));
        return denull(std::move(casted));
    }

    case PathType::PATH_CHECK: {
        std::unique_ptr<PathCheck> casted;
        casted.reset(dynamic_cast<PathCheck*>(path.release()));
        return denull(std::move(casted));
    }

    case PathType::PATH_OPTIONAL: {
        std::unique_ptr<PathOptional> casted;
        casted.reset(dynamic_cast<PathOptional*>(path.release()));
        return denull(std::move(casted));
    }
    case PathType::PATH_NEGATED_SET: {
        std::unique_ptr<PathNegatedSet> casted;
        casted.reset(dynamic_cast<PathNegatedSet*>(path.release()));
        return denull(std::move(casted));
    }
    }
    return nullptr;
}


std::unique_ptr<RegularPathExpr> PathDenull::denull(std::unique_ptr<PathKleeneStar> kleene_star) {
    return std::move(kleene_star->path);
}


std::unique_ptr<RegularPathExpr> PathDenull::denull(std::unique_ptr<PathAlternatives> op_path_alternatives) {
    if (!op_path_alternatives->nullable()) {
        return op_path_alternatives;
    }
    std::vector<std::unique_ptr<RegularPathExpr>> new_alternatives;
    for (size_t i = 0; i < op_path_alternatives->alternatives.size(); i++) {
        new_alternatives.push_back(denull(std::move(op_path_alternatives->alternatives.at(i))));
    }
    return std::make_unique<PathAlternatives>(std::move(new_alternatives));
}


std::unique_ptr<RegularPathExpr> PathDenull::denull(std::unique_ptr<PathSequence> path_sequence) {
    if (!path_sequence->nullable()) {
        return path_sequence;
    }
    std::vector<std::unique_ptr<RegularPathExpr>> new_alternatives;
    for (size_t i = 0; i < path_sequence->sequence.size(); i++) {
        new_alternatives.push_back(denull(std::move(path_sequence->sequence.at(i))));
    }
    return std::make_unique<PathAlternatives>(std::move(new_alternatives));
}


std::unique_ptr<RegularPathExpr> PathDenull::denull(std::unique_ptr<PathNegatedSet> negated_set) {
    return negated_set;
}


std::unique_ptr<RegularPathExpr> PathDenull::denull(std::unique_ptr<PathAtom> atom) {
    return atom;
}


std::unique_ptr<RegularPathExpr> PathDenull::denull(std::unique_ptr<PathOptional> optional) {
    return std::move(optional->path);
}


std::unique_ptr<RegularPathExpr> PathDenull::denull(std::unique_ptr<PathCheck> check) {
    return check;
}
