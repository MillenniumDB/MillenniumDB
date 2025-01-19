#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/common/datatypes/decimal.h"
#include "graph_models/common/datatypes/decimal_inlined.h"

int main() {
    // On this tests the left hand side is always lower than the right hand side
    std::vector<std::pair<std::string, std::string>> tests = {
        { "-11.1111", "-1.10000000000000000000000000000000000000001" },
        { "-1", "-.010000000000000000000000000000000000000001" },
        { "-0.01", "-.000100000000000000000000000000000000001" },

        { "0", ".30303030303030303030303030303030303" },
        { "0", "30303030303030303030303030303030303" },
        { ".01", ".01000000000000000000000000000000000000001" },
        { "1", "1111111111111111111111111111111111111111111111" },
        { "1.1", "1.10000000000000000000000000000000000000001" },
        { "-1.1", "1.10000000000000000000000000000000000000001" },
    };

    bool error;
    for (auto &[inl_str, ext_str] : tests) {
        auto dec_inl1 = Decimal(inl_str, &error);
        assert(!error);
        auto dec_ext1 = Decimal(ext_str, &error);
        assert(!error);

        auto object_id_inl = dec_inl1.to_internal();
        auto object_id_ext = dec_ext1.to_external();

        auto dec_inl2 = SPARQL::Conversions::unpack_decimal(ObjectId(object_id_inl));
        auto dec_ext2 = Decimal::from_external(object_id_ext);

        if (!(dec_inl1 < dec_ext1)) {
            std::cout << inl_str << " < " << ext_str << " should be true (case 1)\n";
            return 1;
        }

        if (!(dec_inl2 < dec_ext2)) {
            std::cout << inl_str << " < " << ext_str << " should be true (case 2)\n";
            return 1;
        }
    }
    return 0;
}
