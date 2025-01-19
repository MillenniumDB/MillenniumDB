#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "query/parser/op/sparql/update/op_create_text_search_index.h"
#include "query/parser/op/sparql/update/op_delete_data.h"
#include "query/parser/op/sparql/update/op_insert_data.h"
#include "storage/index/text_search/text_search_index_update_data.h"

namespace SPARQL {

class UpdateExecutor : public OpVisitor {
public:
    ~UpdateExecutor();

    void visit(SPARQL::OpDeleteData&) override;
    void visit(SPARQL::OpInsertData&) override;
    void visit(SPARQL::OpCreateTextSearchIndex&) override;

    void print_stats(std::ostream& os);

private:
    uint_fast32_t triples_inserted = 0;
    uint_fast32_t triples_deleted = 0;

    std::vector<TextSearchIndexUpdateData> text_search_index_updates;

    std::map<ObjectId, ObjectId> created_ids;

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
};
} // namespace SPARQL
