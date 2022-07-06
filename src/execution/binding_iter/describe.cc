#include "describe.h"

#include "query_optimizer/quad_model/quad_model.h"

using namespace std;

Describe::Describe(unique_ptr<BindingIdIter> labels,
                   unique_ptr<BindingIdIter> properties,
                   unique_ptr<BindingIdIter> outgoing_connections,
                   unique_ptr<BindingIdIter> incoming_conenctions) :
    labels               (move(labels)),
    properties           (move(properties)),
    outgoing_connections (move(outgoing_connections)),
    incoming_connections (move(incoming_conenctions)) { }


void Describe::begin(std::ostream& _os) {
    os = &_os;
    labels->begin(binding_id);
    properties->begin(binding_id);
    outgoing_connections->begin(binding_id);
    incoming_connections->begin(binding_id);
    (*os) << "---------------------------------------\n";
}


bool Describe::next() {
    while (true) {
        switch (status) {
            case 0: {
                if (labels->next() && count_labels < LABEL_LIMIT) {
                    (*os) << quad_model.get_graph_object(binding_id[VarId(0)])
                          << '\n';
                    count_labels++;
                    return true;
                } else {
                    (*os) << "---------------------------------------\n";
                    status = 1;
                    break;
                }
            }
            case 1: {
                if (properties->next() && count_properties < PROPERTY_LIMIT) {
                    (*os) << quad_model.get_graph_object(binding_id[VarId(0)])
                          << ','
                          << quad_model.get_graph_object(binding_id[VarId(1)])
                          << '\n';
                    count_properties++;
                    return true;
                } else {
                    (*os) << "---------------------------------------\n";
                    status = 2;
                    break;
                }
            }
            case 2: {
                if (outgoing_connections->next() && count_outgoing < EDGE_LIMIT) {
                    (*os) << quad_model.get_graph_object(binding_id[VarId(0)])
                          << ','
                          << quad_model.get_graph_object(binding_id[VarId(1)])
                          << ','
                          << quad_model.get_graph_object(binding_id[VarId(2)])
                          << '\n';
                    count_outgoing++;
                    return true;
                } else {
                    (*os) << "---------------------------------------\n";
                    status = 3;
                    break;
                }
            }
            case 3: {
                if (incoming_connections->next() && count_incoming < EDGE_LIMIT) {
                    (*os) << quad_model.get_graph_object(binding_id[VarId(0)])
                          << ','
                          << quad_model.get_graph_object(binding_id[VarId(1)])
                          << ','
                          << quad_model.get_graph_object(binding_id[VarId(2)])
                          << '\n';
                    count_incoming++;
                    return true;
                } else {
                    status = 4;
                    break;
                }
            }
            default:
                return false;
        }
    }
    return false;
}


GraphObject Describe::operator[](VarId /*var_id*/) const {
    throw std::logic_error("Describe is designed to print the result in next() method");
    return GraphObject();
}


void Describe::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Describe()";
}

