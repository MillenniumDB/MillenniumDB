#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"

namespace GQL {
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
    uint64_t count = 0;
};
} // namespace GQL
