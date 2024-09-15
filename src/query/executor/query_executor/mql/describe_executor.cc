#include "describe_executor.h"

#include "graph_models/quad_model/quad_model.h"

using namespace MQL;

uint64_t DescribeExecutor::execute(std::ostream& os) {
    labels->begin(binding);
    properties->begin(binding);
    outgoing_connections->begin(binding);
    incoming_connections->begin(binding);
    os << "---------------------------------------\n";
    while (true) {
        switch (status) {
        case 0: {
            if (count_labels < labels_limit && labels->next()) {
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
            if (count_properties < properties_limit && properties->next()) {
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
            if (count_outgoing < outgoing_limit && outgoing_connections->next()) {
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
            if (count_incoming < incoming_limit && incoming_connections->next()) {
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


void DescribeExecutor::analyze(std::ostream& os, bool, int indent) const {
    os << std::string(indent, ' ') << "DescribeExecutor()";
}
