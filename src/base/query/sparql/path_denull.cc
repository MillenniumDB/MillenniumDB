#include "path_denull.h"

#include <cassert>

#include "base/query/sparql/path.h"
#include "base/query/sparql/path_alternatives.h"
#include "base/query/sparql/path_atom.h"
#include "base/query/sparql/path_kleene_star.h"
#include "base/query/sparql/path_optional.h"
#include "base/query/sparql/path_sequence.h"

using namespace std;
using namespace SPARQL;

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

    case PathType::PATH_ATOM: {
        // IPathAtom
        PathAtom* tmp = dynamic_cast<PathAtom*>(path.get());

        unique_ptr<PathAtom> casted;
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

    default: { // case PathType::PATH_OPTIONAL: {
        PathOptional* tmp = dynamic_cast<PathOptional*>(path.get());

        unique_ptr<PathOptional> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }
    }
}

unique_ptr<IPath> PathDenull::denull(unique_ptr<PathAlternatives> pa) {
    if (!pa->nullable()) {
        return pa;
    }
    vector<unique_ptr<IPath>> new_alternatives;
    for (size_t i = 0; i < pa->alternatives.size(); ++i) {
        new_alternatives.push_back(denull(move(pa->alternatives.at(i))));
    }
    return make_unique<PathAlternatives>(move(new_alternatives));
}

unique_ptr<IPath> PathDenull::denull(unique_ptr<PathSequence> ps) {
    if (!ps->nullable()) {
        return ps;
    }
    vector<unique_ptr<IPath>> new_alternatives;
    for (size_t i = 0; i < ps->sequence.size(); ++i) {
        new_alternatives.push_back(denull(move(ps->sequence.at(i))));
    }
    return make_unique<PathAlternatives>(move(new_alternatives));
}

unique_ptr<IPath> PathDenull::denull(unique_ptr<PathAtom> pa) {
    return pa;
}

unique_ptr<IPath> PathDenull::denull(unique_ptr<PathKleeneStar> pks) {
    return move(pks->path);
}

unique_ptr<IPath> PathDenull::denull(unique_ptr<PathOptional> po) {
    return move(po->path);
}