#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace SPARQL {
class AggGroupConcatDistinct : public Agg {
public:
    using Agg::Agg;
    void begin() override {
        hash_table = std::make_unique<DistinctBindingHash<ObjectId>>(1);
        res = std::string();
        type = UNSET;
    }

    void process() override {
        if (type == NULL_RESULT) {
            return;
        }

        auto oid = expr->eval(*binding);

        oid_vec[0] = oid;
        if (hash_table->is_in_or_insert(oid_vec)) {
            return;
        }

        if (type != UNSET) {
            res += sep;
        }

        switch (oid.get_sub_type()) {
        case ObjectId::MASK_STRING_SIMPLE: {
            auto str = Conversions::unpack_string_simple(oid);
            res += str;
            type = SIMPLE;
            break;
        }
        case ObjectId::MASK_STRING_LANG: {
            auto [lang, str] = Conversions::unpack_string_lang(oid);
            res += str;
            if (type == UNSET) {
                type = lang;
            } else if (lang != type) {
                type = SIMPLE;
            }
            break;
        }
        default: {
        }
            type = NULL_RESULT;
        }
    }

    // indicates the end of a group
    ObjectId get() override {
        switch (type) {
        case UNSET:
            return ObjectId(ObjectId::STRING_SIMPLE_EMPTY);
        case NULL_RESULT:
            return ObjectId::get_null();
        case SIMPLE:
            return Conversions::pack_string_simple(res);
        default:
            return Conversions::pack_string_lang(type, res);
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "GROUPCONCAT(DISTINCT " << *expr << ")";
        return os;
    }

    std::string sep;
private:
    int32_t type; // -3 unset, -2 null result, -1 simple, >=0 lang
    constexpr static int32_t UNSET       = -3;
    constexpr static int32_t NULL_RESULT = -2;
    constexpr static int32_t SIMPLE      = -1;
    std::string res;

    std::unique_ptr<DistinctBindingHash<ObjectId>> hash_table;

    // Vector to pass oid to the hash table
    std::vector<ObjectId> oid_vec{1};
};
} // namespace SPARQL
