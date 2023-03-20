#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base/binding/binding_id_iter.h"
#include "base/binding/binding_iter.h"
#include "base/query/var.h"

class Describe : public BindingIter {
public:
    Describe(std::unique_ptr<BindingIdIter> labels,
             std::unique_ptr<BindingIdIter> properties,
             std::unique_ptr<BindingIdIter> outgoing_connections,
             std::unique_ptr<BindingIdIter> incoming_conenctions);

    void begin(std::ostream&) override;

    bool next() override;

    GraphObject operator[](VarId var_id) const override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    // Setted when begin is called. Used to print separator when changing status
    std::ostream* os;

    BindingId binding_id = BindingId(3);

    // 0: label, 1: properties, 2: outer connections, 3: inner connections
    int status = 0;

    std::unique_ptr<BindingIdIter> labels;
    std::unique_ptr<BindingIdIter> properties;
    std::unique_ptr<BindingIdIter> outgoing_connections;
    std::unique_ptr<BindingIdIter> incoming_connections;

    // TODO: remove limits? they exist because the frontend breaks when handling too many results
    static constexpr uint_fast32_t LABEL_LIMIT = 100;
    static constexpr uint_fast32_t PROPERTY_LIMIT = 100;
    static constexpr uint_fast32_t EDGE_LIMIT = 1000;

    uint_fast32_t count_labels = 0;
    uint_fast32_t count_properties = 0;
    uint_fast32_t count_outgoing = 0;
    uint_fast32_t count_incoming = 0;
};
