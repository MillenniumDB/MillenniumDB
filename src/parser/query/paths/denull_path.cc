#include "denull_path.h"

#include <cassert>

#include "parser/query/paths/path.h"
#include "parser/query/paths/path_alternatives.h"
#include "parser/query/paths/path_atom.h"
#include "parser/query/paths/path_kleene_star.h"
#include "parser/query/paths/path_optional.h"
#include "parser/query/paths/path_sequence.h"

using namespace std;

unique_ptr<IPath> PathDenull::denull(unique_ptr<IPath> path) {
    auto res = accept_denull(move(path));
    assert(res != nullptr);
    return res;
}


unique_ptr<IPath> PathDenull::accept_denull(unique_ptr<IPath> path) {
    auto type = path->type();

    switch (type) {
    case PathType::PATH_ALTERNATIVES: {
        PathAlternatives* tmp = dynamic_cast<PathAlternatives*>(path.get());

        unique_ptr<PathAlternatives> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }

    case PathType::PATH_SEQUENCE: {
        PathSequence* tmp = dynamic_cast<PathSequence*>(path.get());

        unique_ptr<PathSequence> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }

    case PathType::PATH_KLEENE_STAR: {
        PathKleeneStar* tmp = dynamic_cast<PathKleeneStar*>(path.get());

        unique_ptr<PathKleeneStar> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }

    case PathType::PATH_ATOM: {
        // IPathAtom
        PathAtom* tmp = dynamic_cast<PathAtom*>(path.get());

        unique_ptr<PathAtom> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }

    default: { // case PathType::PATH_OPTIONAL: {
        PathOptional* tmp = dynamic_cast<PathOptional*>(path.get());

        unique_ptr<PathOptional> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }
    }
}


unique_ptr<IPath> PathDenull::denull(unique_ptr<PathKleeneStar> kleene_star) {
    return move(kleene_star->path);
}


unique_ptr<IPath> PathDenull::denull(unique_ptr<PathAlternatives> op_path_alternatives) {
    if (!op_path_alternatives->nullable()) {
        return op_path_alternatives;
    }
    vector<unique_ptr<IPath>> new_alternatives;
    for (size_t i = 0; i < op_path_alternatives->alternatives.size(); i++) {
        new_alternatives.push_back(denull(move(op_path_alternatives->alternatives.at(i))));
    }
    return make_unique<PathAlternatives>(move(new_alternatives));
}


unique_ptr<IPath> PathDenull::denull(unique_ptr<PathSequence> path_sequence) {
    if (!path_sequence->nullable()) {
        return path_sequence;
    }
    vector<unique_ptr<IPath>> new_alternatives;
    for (size_t i = 0; i < path_sequence->sequence.size(); i++) {
        new_alternatives.push_back(denull(move(path_sequence->sequence.at(i))));
    }
    return make_unique<PathAlternatives>(move(new_alternatives));
}


unique_ptr<IPath> PathDenull::denull(unique_ptr<PathAtom> atom) {
    return atom;
}


unique_ptr<IPath> PathDenull::denull(unique_ptr<PathOptional> optional) {
    return move(optional->path);
}
