#include <iostream>
#include <memory>

#include "execution/binding_id_iter/paths/any_shortest/experimental/bfs_smt_enum.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "parser/query/paths/path_alternatives.h"
// #include "parser/query/paths/path_atom.h"
#include "parser/query/paths/path_atom_smt.h"
#include "parser/query/paths/path_kleene_star.h"
#include "parser/query/paths/path_optional.h"
#include "parser/query/paths/path_sequence.h"
#include "parser/query/paths/path_check.h"
#include "storage/buffer_manager.h"

using namespace std;

// returns the automaton for the rpq ":p1/:p2*"
SMTAutomaton get_automaton() {
    // This method manually constructs the automaton
    // the simplest transition will be `PathAtomSMT`
    // and you can construct a complex automaton using other operators:
    // PathKleeneStar: (e)*
    // PathOptional: (e)?
    // PathSequence: (e_1 / ... / e_n), '/' is symbol for concatenation
    // PathAlternatives: (e_1 | ... | e_n), '|' is symbol for or

    // This example is PathSequence(PathAtom("p1"), PathKleeneStar(PathAtom("p2"))),
    // in our query language this would be expressed as ":p1/:p2*"

    // use false as second parameter PathAtomSMT, this is the edge direction, false means forward.
    auto p1 = make_unique<PathAtomSMT>("p1", false); // TODO: pass formulas as third parameter
    auto p2 = make_unique<PathAtomSMT>("p2", false); // TODO: pass formulas as third parameter
    auto p2_kleene = make_unique<PathKleeneStar>(move(p2));
    auto sequence_elements = vector<unique_ptr<IPath>>();
    sequence_elements.push_back(move(p1));
    sequence_elements.push_back(move(p2_kleene));

    PathSequence seq(move(sequence_elements));

    std::function<ObjectId(const std::string&)> str_to_object_id_f = [](const std::string& str) {
        return quad_model.get_object_id(GraphObject::make_named_node(str));
    };

    return seq.get_smt_automaton(str_to_object_id_f);
}


int main() {
    // TODO: remember to create DB before executing this test
    // build/Release/bin/create_db tests/dbs/test_db.txt tests/dbs/test_db
    string db_folder = "tests/dbs/test_db";
    auto model_destroyer = QuadModel::init(db_folder,
                                           BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE,
                                           BufferManager::DEFAULT_PRIVATE_BUFFER_POOL_SIZE,
                                           1);

    ThreadInfo thread_info(chrono::system_clock::now()); // Timeout is not considered in this test

    ObjectId start_node = quad_model.get_object_id(GraphObject::make_named_node("N1"));

    VarId path_var(0);
    VarId reached_node(1);
    BindingId binding_id(2);

    auto automaton = get_automaton();

    auto iter = make_unique<Paths::AnyShortest::SMTEnum>(&thread_info,
                                                         path_var,
                                                         start_node,
                                                         reached_node,
                                                         automaton);

    // Execution
    path_manager.begin(binding_id.size, false);
    iter->begin(binding_id);
    while (iter->next()) {
        cout << quad_model.get_graph_object(binding_id[path_var]) << '\n';
    }
    path_manager.clear();
}
