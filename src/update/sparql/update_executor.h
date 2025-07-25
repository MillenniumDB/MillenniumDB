#pragma once

#include <cstdint>
#include <map>
#include <string>

#include <boost/unordered/unordered_map.hpp>

#include "query/parser/op/sparql/update/op_create_hnsw_index.h"
#include "query/parser/op/sparql/update/op_create_text_index.h"
#include "query/parser/op/sparql/update/op_delete_data.h"
#include "query/parser/op/sparql/update/op_insert_data.h"
#include "storage/index/hnsw/hnsw_index_update_data.h"
#include "storage/index/text_search/text_index_update_data.h"

namespace SPARQL {

class UpdateExecutor : public OpVisitor {
public:
    struct GraphUpdateData {
        uint_fast32_t triples_inserted { 0 };
        uint_fast32_t triples_deleted { 0 };

        bool empty() const
        {
            return triples_inserted == 0 && triples_deleted == 0;
        }

        friend std::ostream& operator<<(std::ostream& os, const GraphUpdateData& data)
        {
            os << "{\"triples_inserted\": " << data.triples_inserted;
            os << ", \"triples_deleted\": " << data.triples_deleted << "}";
            return os;
        }
    };

    ~UpdateExecutor();

    void visit(OpDeleteData&) override;
    void visit(OpInsertData&) override;
    void visit(OpCreateTextIndex&) override;
    void visit(OpCreateHNSWIndex&) override;

    void visit(OpUpdate&) override { }

    // Not updates
    void visit(OpAsk&) override { }
    void visit(OpBasicGraphPattern&) override { }
    void visit(OpBind&) override { }
    void visit(OpConstruct&) override { }
    void visit(OpDescribe&) override { }
    void visit(OpEmpty&) override { }
    void visit(OpFrom&) override { }
    void visit(OpFilter&) override { }
    void visit(OpGraph&) override { }
    void visit(OpGroupBy&) override { }
    void visit(OpHaving&) override { }
    void visit(OpJoin&) override { }
    void visit(OpMinus&) override { }
    void visit(OpNotExists&) override { }
    void visit(OpOptional&) override { }
    void visit(OpOrderBy&) override { }
    void visit(OpPath&) override { }
    void visit(OpProcedure&) override { }
    void visit(OpSelect&) override { }
    void visit(OpSequence&) override { }
    void visit(OpSemiJoin&) override { }
    void visit(OpService&) override { }
    void visit(OpShow&) override { }
    void visit(OpTriple&) override { }
    void visit(OpUnion&) override { }
    void visit(OpUnitTable&) override { }
    void visit(OpValues&) override { }

    void print_stats(std::ostream& os);

private:
    std::map<ObjectId, ObjectId> created_ids;

    GraphUpdateData graph_update_data;

    boost::unordered_map<std::string, TextIndexUpdateData> name2text_search_index_update_data;
    boost::unordered_map<std::string, HNSWIndexUpdateData> name2hnsw_index_update_data;

    // returns true if oid was transformed
    bool transform_if_tmp(ObjectId& oid);

    // helper for string with lang and string with datatype.
    // returns true if oid was transformed
    bool try_transform_inline(ObjectId& oid, std::vector<std::string>& catalog_list, char split);

    // helper for string with lang and string with datatype.
    // returns true if oid was transformed
    bool try_transform_tmp(ObjectId& oid, std::vector<std::string>& catalog_list, char split);

    // helper for string with lang and string with datatype.
    // returns true if oid was transformed
    bool try_transform_extern(ObjectId& oid, std::vector<std::string>& catalog_list, char split);

    void insert_text_search_index_update_data(TextIndexUpdateData&& text_search_index_update_data);

    void insert_hnsw_index_update_data(HNSWIndexUpdateData&& hnsw_index_update_data);
};
} // namespace SPARQL
