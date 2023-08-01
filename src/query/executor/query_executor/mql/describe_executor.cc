#include "describe_executor.h"

#include "graph_models/quad_model/quad_model.h"

using namespace MQL;

uint64_t DescribeExecutor::execute(std::ostream& os) {
    os << "---------------------------------------\n";
    while (true) {
        switch (status) {
        case 0: {
            if (labels->next() && count_labels < labels_limit) {
                os << binding[VarId(0)]
                   << '\n';
                count_labels++;
            } else {
                os << "---------------------------------------\n";
                status = 1;
            }
            break;
        }
        case 1: {
            if (properties->next() && count_properties < properties_limit) {
                os << binding[VarId(0)]
                   << ','
                   << binding[VarId(1)]
                   << '\n';
                count_properties++;
            } else {
                os << "---------------------------------------\n";
                status = 2;
            }
            break;
        }
        case 2: {
            if (outgoing_connections->next() && count_outgoing < outgoing_limit) {
                os << binding[VarId(0)]
                   << ','
                   << binding[VarId(1)]
                   << ','
                   << binding[VarId(2)]
                   << '\n';
                count_outgoing++;
            } else {
                os << "---------------------------------------\n";
                status = 3;
            }
            break;
        }
        case 3: {
            if (incoming_connections->next() && count_incoming < incoming_limit) {
                os << binding[VarId(0)]
                   << ','
                   << binding[VarId(1)]
                   << ','
                   << binding[VarId(2)]
                   << '\n';
                count_incoming++;
            } else {
                status = 4;
            }
            break;
        }
        default:
            return count_labels + count_properties + count_outgoing + count_incoming;
        }
    }
}


void DescribeExecutor::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "DescribeExecutor()";
}
