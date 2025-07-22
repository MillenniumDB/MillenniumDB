#pragma once

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"

namespace MQL {
class AggCountAll : public Agg {
public:
    using Agg::Agg;

    void begin() override
    {
        count = 0;
    }

    void process() override
    {
        count++;
    }

    // indicates the end of a group
    ObjectId get() override
    {
        return Conversions::pack_int(count);
    }

    std::ostream& print(std::ostream& os) const override
    {
        os << "COUNT(*)";
        return os;
    }

private:
    int64_t count = 0;
};
} // namespace MQL
