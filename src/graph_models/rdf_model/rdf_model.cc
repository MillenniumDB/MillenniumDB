#include "rdf_model.h"

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/sparql_binding_expr_printer.h"
#include "storage/index/bplus_tree/bplus_tree.h"

using namespace std;

// memory for the object
static typename std::aligned_storage<sizeof(RdfModel), alignof(RdfModel)>::type rdf_model_buf;
// global object
RdfModel& rdf_model = reinterpret_cast<RdfModel&>(rdf_model_buf);


std::unique_ptr<ModelDestroyer> RdfModel::init() {
    new (&rdf_model) RdfModel();
    return std::make_unique<ModelDestroyer>([]() { rdf_model.~RdfModel(); });
}


std::unique_ptr<BindingExprPrinter> create_rdf_binding_expr_printer(std::ostream& os) {
    return std::make_unique<SPARQL::BindingExprPrinter>(os);
}


RdfModel::RdfModel() :
    m_catalog("catalog.dat")
{
    QueryContext::_debug_print = SPARQL::Conversions::debug_print;
    QueryContext::create_binding_expr_printer = create_rdf_binding_expr_printer;

    spo = make_unique<BPlusTree<3>>("spo");
    pos = make_unique<BPlusTree<3>>("pos");
    osp = make_unique<BPlusTree<3>>("osp");

    if (catalog().permutations >= 4) {
        pso = make_unique<BPlusTree<3>>("pso");
    }

    if (catalog().permutations == 6) {
        sop = make_unique<BPlusTree<3>>("sop");
        ops = make_unique<BPlusTree<3>>("ops");
    }

    equal_spo = make_unique<BPlusTree<1>>("equal_spo");
    equal_sp  = make_unique<BPlusTree<2>>("equal_sp");
    equal_so  = make_unique<BPlusTree<2>>("equal_so");
    equal_po  = make_unique<BPlusTree<2>>("equal_po");

    equal_sp_inverted = make_unique<BPlusTree<2>>("equal_sp_inverted");
    equal_so_inverted = make_unique<BPlusTree<2>>("equal_so_inverted");
    equal_po_inverted = make_unique<BPlusTree<2>>("equal_po_inverted");
}
