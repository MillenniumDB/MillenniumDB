#ifndef RELATIONAL_MODEL__MATCH_H_
#define RELATIONAL_MODEL__MATCH_H_

#include <memory>

#include "base/binding/binding_id_iter.h"
#include "base/binding/binding_iter.h"
#include "base/graph/graph_model.h"
#include "relational_model/execution/binding/binding_materialize_id.h"

class Match : public BindingIter {
public:
    Match(const GraphModel& model, std::unique_ptr<BindingIdIter> root, size_t binding_size);
    ~Match() = default;

    inline Binding& get_binding() noexcept override { return my_binding; }

    void begin() override;
    bool next() override;
    void analyze(std::ostream&, int indent = 0) const override;

private:
    const GraphModel& model;
    std::unique_ptr<BindingIdIter> root;
    BindingId input;
    BindingId* binding_id_root;

    BindingMaterializeId my_binding;
};

#endif // RELATIONAL_MODEL__MATCH_H_
