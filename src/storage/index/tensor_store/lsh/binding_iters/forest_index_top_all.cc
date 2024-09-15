#include "forest_index_top_all.h"

#include "graph_models/quad_model/conversions.h"
#include "storage/index/tensor_store/lsh/forest_index_query_iter.h"

using namespace LSH;

ForestIndexTopAll::ForestIndexTopAll(VarId object_var_,
                                     VarId similarity_var_,
                                     std::unique_ptr<ForestIndexQueryIter> query_iter_) :
    object_var     (object_var_),
    similarity_var (similarity_var_),
    query_iter     (std::move(query_iter_)) { }


void ForestIndexTopAll::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


void ForestIndexTopAll::_begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;
    query_iter->begin();
}


bool ForestIndexTopAll::_next() {
    if (query_iter->next()){
        parent_binding->add(object_var, ObjectId((*query_iter->current).first));
        parent_binding->add(similarity_var, MQL::Conversions::pack_float((*query_iter->current).second));
        return true;
    }
    return false;
}


void ForestIndexTopAll::_reset() {
    query_iter->reset();
}


void ForestIndexTopAll::assign_nulls() {
    parent_binding->add(object_var, ObjectId::get_null());
    parent_binding->add(similarity_var, ObjectId::get_null());
}