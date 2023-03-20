#include <iostream>

#include "query_optimizer/rdf_model/rdf_model.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/record.h"

using namespace std;

int main() {
    int    shared_buffer_size  = 1024;
    int    private_buffer_size = 1024;
    string db_folder           = "tests/dbs/nt1";

    ThreadInfo thread_info;

    auto model_destroyer = RdfModel::init(db_folder, shared_buffer_size, private_buffer_size, 1);

    auto iter = rdf_model.spo->get_range(&thread_info.interruption_requested,
                                         RecordFactory::get(0, 0, 0),
                                         RecordFactory::get(UINT64_MAX, UINT64_MAX, UINT64_MAX));

    for (auto record = iter->next(); record != nullptr; record = iter->next()) {
        cout << rdf_model.get_graph_object(ObjectId(record->ids[0])) << ' '
             << rdf_model.get_graph_object(ObjectId(record->ids[1])) << ' '
             << rdf_model.get_graph_object(ObjectId(record->ids[2])) << " ."
             << endl;
    }

    return 0;
}