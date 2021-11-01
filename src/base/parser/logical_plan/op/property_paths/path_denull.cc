#include "path_denull.h"

#include <cassert>

#include "base/parser/logical_plan/op/op_path.h"
#include "base/parser/logical_plan/op/op_path_alternatives.h"
#include "base/parser/logical_plan/op/op_path_sequence.h"
#include "base/parser/logical_plan/op/op_path_atom.h"
#include "base/parser/logical_plan/op/op_path_kleene_star.h"
#include "base/parser/logical_plan/op/op_path_optional.h"

using namespace std;

unique_ptr<OpPath> PathDenull::denull(unique_ptr<OpPath> path) {
    auto res = accept_denull( move(path) );
    assert(res != nullptr);
    return res;
}


unique_ptr<OpPath> PathDenull::accept_denull(unique_ptr<OpPath> path) {
    auto type = path->type();

    switch (type) {
    case OpPathType::OP_PATH_ALTERNATIVES : {

        // OpPathAlternatives
        OpPathAlternatives* tmp = dynamic_cast<OpPathAlternatives*>(path.get());
        std::unique_ptr<OpPathAlternatives> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }

    case OpPathType::OP_PATH_SEQUENCE : {
        // OpPathSequence
        OpPathSequence* tmp = dynamic_cast<OpPathSequence*>(path.get());
        std::unique_ptr<OpPathSequence> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }

    case OpPathType::OP_PATH_KLEENE_STAR : {
        // OpPathKleenStar
        OpPathKleeneStar* tmp = dynamic_cast<OpPathKleeneStar*>(path.get());
        std::unique_ptr<OpPathKleeneStar> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }

    case OpPathType::OP_PATH_ATOM : {
        // OpPathAtom
        OpPathAtom* tmp = dynamic_cast<OpPathAtom*>(path.get());
        std::unique_ptr<OpPathAtom> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }

    default : { //OpPathType::op_path_optional : {
        // OpPathOptional
        OpPathOptional* tmp = dynamic_cast<OpPathOptional*>(path.get());
        std::unique_ptr<OpPathOptional> casted;
        path.release();
        casted.reset(tmp);
        return denull(move(casted));
    }
    }
}


unique_ptr<OpPath> PathDenull::denull(unique_ptr<OpPathKleeneStar> kleene_star) {
    return move( kleene_star->path ); // ((:P1*)*)*
}


unique_ptr<OpPath> PathDenull::denull(unique_ptr<OpPathAlternatives> op_path_alternatives) {
    if (!op_path_alternatives->nullable()) {
        return op_path_alternatives;
    }
    vector<unique_ptr<OpPath>> new_alternatives;
    for (size_t i = 0; i < op_path_alternatives->alternatives.size(); i++) {
        new_alternatives.push_back( denull(move(op_path_alternatives->alternatives.at(i))) );
    }
    return make_unique<OpPathAlternatives>(move(new_alternatives));
}


unique_ptr<OpPath> PathDenull::denull(unique_ptr<OpPathSequence> path_sequence) {
    if (!path_sequence->nullable()) {
        return path_sequence;
    }
    vector<unique_ptr<OpPath>> new_alternatives;
    for (size_t i = 0; i < path_sequence->sequence.size(); i++) {
        new_alternatives.push_back( denull(move(path_sequence->sequence.at(i))) );
    }
    return make_unique<OpPathAlternatives>(move(new_alternatives));

}

unique_ptr<OpPath> PathDenull::denull(unique_ptr<OpPathAtom> atom) {
    return atom;
}

unique_ptr<OpPath> PathDenull::denull(unique_ptr<OpPathOptional> optional) {
    return move(optional->path);
}
