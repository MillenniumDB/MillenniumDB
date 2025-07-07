#include "update_executor.h"

#include <iostream>
#include <sstream>

#include "graph_models/inliner.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/text_index.h"
#include "storage/index/text_search/text_index_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

using namespace SPARQL;

UpdateExecutor::~UpdateExecutor()
{
    // TODO: force string file WAL flush?
}

constexpr uint64_t CLEAR_TMP_MASK = ~(ObjectId::MOD_MASK | ObjectId::MASK_EXTERNAL_ID);
constexpr uint64_t CLEAR_TAG_MASK = ~(ObjectId::MASK_LITERAL_TAG);

// helper to generalize inline langs and inline datatypes
bool UpdateExecutor::try_transform_inline(ObjectId& oid, std::vector<std::string>& catalog_list, char split)
{
    auto old_tag_id = oid.get_value() >> Conversions::TMP_SHIFT;

    // if catalog doesn't have space left
    // or old_tag_id is not the tmp special value (tag is in catalog)
    if (catalog_list.size() >= RdfCatalog::MAX_LANG_AND_DTT
        || old_tag_id != (Conversions::LAST_TMP_ID >> Conversions::TMP_SHIFT))
    {
        return false;
    }
    const uint8_t* data = reinterpret_cast<uint8_t*>(&oid.id);
    char str_b[6] = {}; // 5 + 1 for null byte, zero initialized

    for (size_t i = 0; i < 5; i++) {
        str_b[i] = data[4 - i];
    }

    std::string tmp_str = std::string(str_b);

    auto found = tmp_str.find_last_of(split);
    if (found == std::string::npos) {
        throw LogicException(
            "try_transform_inline: string with LAST_TMP_ID `" + tmp_str + "` must have " + split
            + " as separator"
        );
    }
    std::string new_str = tmp_str.substr(0, found);
    std::string suffix = tmp_str.substr(found + 1);

    // search in catalog. maybe was inserted by previous operation
    uint64_t new_tag_id = 0;
    for (const auto& catalog_item : catalog_list) {
        if (catalog_item == suffix) {
            break;
        } else {
            new_tag_id++;
        }
    }

    if (new_tag_id == catalog_list.size()) {
        catalog_list.push_back(suffix);
    }

    auto new_inline_id = Inliner::inline_string5(new_str.c_str());

    // update tag id
    oid.id = (oid.id & CLEAR_TAG_MASK) | (new_tag_id << Conversions::TMP_SHIFT);

    // update external id
    oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_INLINE | new_inline_id;

    return true;
}

bool UpdateExecutor::try_transform_tmp(ObjectId& oid, std::vector<std::string>& catalog_list, char split)
{
    auto tmp_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    auto& tmp_str = tmp_manager.get_str(tmp_id);
    auto old_tag_id = oid.get_value() >> Conversions::TMP_SHIFT;

    // if catalog doesn't have space left
    // or old_tag_id is not the tmp special value (tag is in catalog)
    if (catalog_list.size() >= RdfCatalog::MAX_LANG_AND_DTT
        || old_tag_id != (Conversions::LAST_TMP_ID >> Conversions::TMP_SHIFT))
    {
        // we don't have to modify the tag, but we need to materialize the string
        auto new_external_id = string_manager.get_or_create(tmp_str.data(), tmp_str.size());

        oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_EXTERNAL | new_external_id;
        return false;
    }

    auto found = tmp_str.find_last_of(split);
    if (found == std::string::npos) {
        throw LogicException(
            "try_transform_tmp: string with LAST_TMP_ID `" + tmp_str + "` must have " + split
            + " as separator"
        );
    }
    std::string new_str = tmp_str.substr(0, found);
    std::string suffix = tmp_str.substr(found + 1);

    // search in catalog. maybe was inserted by previous operation
    uint64_t new_tag_id = 0;
    for (const auto& catalog_item : catalog_list) {
        if (catalog_item == suffix) {
            break;
        } else {
            new_tag_id++;
        }
    }

    if (new_tag_id == catalog_list.size()) {
        catalog_list.push_back(suffix);
    }

    auto new_external_id = string_manager.get_or_create(new_str.data(), new_str.size());

    // update tag id
    oid.id = (oid.id & CLEAR_TAG_MASK) | (new_tag_id << Conversions::TMP_SHIFT);

    // update external id
    oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_EXTERNAL | new_external_id;

    return true;
}

bool UpdateExecutor::try_transform_extern(ObjectId& oid, std::vector<std::string>& catalog_list, char split)
{
    auto old_tag_id = oid.get_value() >> Conversions::TMP_SHIFT;

    // if catalog doesn't have space left
    // or old_tag_id is not the tmp special value (tag is in catalog)
    if (catalog_list.size() >= RdfCatalog::MAX_LANG_AND_DTT
        || old_tag_id != (Conversions::LAST_TMP_ID >> Conversions::TMP_SHIFT))
    {
        return false;
    }

    auto extern_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    std::stringstream ss;
    string_manager.print(ss, extern_id);
    auto old_str = ss.str();

    auto found = old_str.find_last_of(split);
    if (found == std::string::npos) {
        throw LogicException(
            "try_transform_extern string with LAST_TMP_ID `" + old_str + "` must have " + split
            + " as separator"
        );
    }
    std::string new_str = old_str.substr(0, found);
    std::string suffix = old_str.substr(found + 1);

    // search in catalog. maybe was inserted by previous operation
    uint64_t new_tag_id = 0;
    for (const auto& catalog_item : catalog_list) {
        if (catalog_item == suffix) {
            break;
        } else {
            new_tag_id++;
        }
    }

    if (new_tag_id == catalog_list.size()) {
        catalog_list.push_back(suffix);
    }

    auto new_external_id = string_manager.get_or_create(new_str.data(), new_str.size());

    // update tag id
    oid.id = (oid.id & CLEAR_TAG_MASK) | (new_tag_id << Conversions::TMP_SHIFT);

    // update external id
    oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_EXTERNAL | new_external_id;
    return true;
}

// If the language or datatype is tmp and can be put into the catalog the
// oid is modified and the catalog is updated.
// If the tmp_manager is in use, move to the external manager
bool UpdateExecutor::transform_if_tmp(ObjectId& oid)
{
    auto oid_type = RDF_OID::get_type(oid);

    switch (oid_type) {
    case RDF_OID::Type::STRING_LANG_INLINE: {
        return try_transform_inline(oid, rdf_model.catalog.languages, '@');
    }
    case RDF_OID::Type::STRING_LANG_EXTERN: {
        return try_transform_extern(oid, rdf_model.catalog.languages, '@');
    }
    case RDF_OID::Type::STRING_LANG_TMP: {
        return try_transform_tmp(oid, rdf_model.catalog.languages, '@');
    }
    case RDF_OID::Type::STRING_DATATYPE_INLINE: {
        return try_transform_inline(oid, rdf_model.catalog.datatypes, '^');
    }
    case RDF_OID::Type::STRING_DATATYPE_EXTERN: {
        return try_transform_extern(oid, rdf_model.catalog.datatypes, '^');
    }
    case RDF_OID::Type::STRING_DATATYPE_TMP: {
        return try_transform_tmp(oid, rdf_model.catalog.datatypes, '^');
    }
    default: {
        if (oid.is_tmp()) {
            const uint64_t tmp_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
            const auto& tmp_str = tmp_manager.get_str(tmp_id);

            const uint64_t gen_t = oid.id & ObjectId::GENERIC_TYPE_MASK;

            uint64_t new_external_id;
            if (gen_t == ObjectId::MASK_TENSOR) {
                new_external_id = tensor_manager.get_or_create_id(tmp_str.data(), tmp_str.size());
            } else {
                new_external_id = string_manager.get_or_create(tmp_str.data(), tmp_str.size());
            }

            oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_EXTERNAL | new_external_id;
        }

        return true;
    }
    }
}

void UpdateExecutor::visit(SPARQL::OpInsertData& op_insert_data)
{
    // Store all the inserts that will be necessary for the indexes
    using Name2InsertsMap = boost::unordered_map<std::string, std::vector<std::tuple<ObjectId, ObjectId>>>;
    const auto& text_index_predicate2names = rdf_model.catalog.text_index_manager
                                                        .get_predicate2names();
    Name2InsertsMap text_index_name2inserts;

    const auto& hnsw_index_predicate2names = rdf_model.catalog.hnsw_index_manager.get_predicate2names();
    Name2InsertsMap hnsw_index_name2inserts;

    // to receive the data
    for (auto& triple : op_insert_data.triples) {
        assert(triple.subject.is_OID());
        assert(triple.predicate.is_OID());
        assert(triple.object.is_OID());

        auto S = triple.subject.get_OID();
        auto P = triple.predicate.get_OID();
        auto O = triple.object.get_OID();

        auto originalS = S;
        auto originalP = P;
        auto originalO = O;

        if (transform_if_tmp(S)) {
            created_ids.insert({ originalS, S });
        }
        if (transform_if_tmp(P)) {
            created_ids.insert({ originalP, P });
        }
        if (transform_if_tmp(O)) {
            created_ids.insert({ originalO, O });
        }

        assert(!S.is_tmp());
        assert(!P.is_tmp());
        assert(!O.is_tmp());

        Record<3> record_spo = { S.id, P.id, O.id };
        bool is_new_record = rdf_model.spo->insert(record_spo);

        if (is_new_record) {
            if (!text_index_predicate2names.empty() || !hnsw_index_predicate2names.empty()) {
                const auto predicate_str = SPARQL::Conversions::unpack_iri(P);
                { // register text index inserts
                    const auto it = text_index_predicate2names.find(predicate_str);
                    if (it != text_index_predicate2names.end()) {
                        for (const auto& name : it->second) {
                            text_index_name2inserts[name].emplace_back(S, O);
                        }
                    }
                }
                { // register hnsw index inserts
                    const auto it = hnsw_index_predicate2names.find(predicate_str);
                    if (it != hnsw_index_predicate2names.end()) {
                        for (const auto& name : it->second) {
                            hnsw_index_name2inserts[name].emplace_back(S, O);
                        }
                    }
                }
            }

            rdf_model.catalog.insert_triple(S.id, P.id, O.id);
            graph_update_data.triples_inserted++;

            Record<3> record_pos = { P.id, O.id, S.id };
            rdf_model.pos->insert(record_pos);

            Record<3> record_osp = { O.id, S.id, P.id };
            rdf_model.osp->insert(record_osp);

            if (rdf_model.pso != nullptr) {
                Record<3> record_pso = { P.id, S.id, O.id };
                rdf_model.pso->insert(record_pso);
            }

            if (rdf_model.sop != nullptr) {
                Record<3> record_sop = { S.id, O.id, P.id };
                rdf_model.sop->insert(record_sop);
            }

            if (rdf_model.ops != nullptr) {
                Record<3> record_ops = { O.id, P.id, S.id };
                rdf_model.ops->insert(record_ops);
            }

            if (S == P) {
                Record<2> record_eq_sp = { S.id, O.id };
                rdf_model.equal_sp->insert(record_eq_sp);

                Record<2> record_eq_sp_inv = { O.id, S.id };
                rdf_model.equal_sp_inverted->insert(record_eq_sp_inv);

                if (P == O) {
                    Record<1> record_eq_all = { S.id };
                    rdf_model.equal_spo->insert(record_eq_all);
                }
            }
            if (S == O) {
                Record<2> record_eq_so = { S.id, P.id };
                rdf_model.equal_sp->insert(record_eq_so);

                Record<2> record_eq_so_inv = { P.id, S.id };
                rdf_model.equal_sp_inverted->insert(record_eq_so_inv);
            }
            if (P == O) {
                Record<2> record_eq_po = { P.id, S.id };
                rdf_model.equal_sp->insert(record_eq_po);

                Record<2> record_eq_po_inv = { S.id, P.id };
                rdf_model.equal_sp_inverted->insert(record_eq_po_inv);
            }
        }
    }

    // Execute index inserts
    for (const auto& [name, inserts] : text_index_name2inserts) {
        auto* text_search_index_ptr = rdf_model.catalog.text_index_manager.get_text_index(name);
        assert(text_search_index_ptr != nullptr && "Text index not found");

        TextIndexUpdateData tsi_update {};
        tsi_update.index_name = name;

        for (const auto& [S, O] : inserts) {
            const auto current_inserted_tokens = text_search_index_ptr->index_single(S, O);
            if (current_inserted_tokens > 0) {
                ++tsi_update.inserted_elements;
                tsi_update.inserted_tokens += current_inserted_tokens;
            }
        }

        insert_text_search_index_update_data(std::move(tsi_update));
    }

    for (const auto& [name, inserts] : hnsw_index_name2inserts) {
        auto* hnsw_index_ptr = rdf_model.catalog.hnsw_index_manager.get_hnsw_index(name);
        assert(hnsw_index_ptr != nullptr && "HNSW index not found");

        HNSWIndexUpdateData hi_update {};
        hi_update.index_name = name;

        for (const auto& [S, O] : inserts) {
            const bool was_inserted = hnsw_index_ptr->index_single<true>(S, O);
            if (was_inserted) {
                ++hi_update.inserted_elements;
            }
        }

        insert_hnsw_index_update_data(std::move(hi_update));
    }
}

void UpdateExecutor::visit(SPARQL::OpDeleteData& op_delete_data)
{
    // Store all the deletes that will be necessary for the indexes
    using Name2DeletesMap = boost::unordered_map<std::string, std::vector<std::tuple<ObjectId, ObjectId>>>;
    const auto& text_index_predicate2names = rdf_model.catalog.text_index_manager
                                                        .get_predicate2names();
    Name2DeletesMap text_index_name2deletes;

    const auto& hnsw_index_predicate2names = rdf_model.catalog.hnsw_index_manager.get_predicate2names();
    Name2DeletesMap hnsw_index_name2deletes;

    for (auto& triple : op_delete_data.triples) {
        assert(triple.subject.is_OID());
        assert(triple.predicate.is_OID());
        assert(triple.object.is_OID());

        auto S = triple.subject.get_OID();
        auto P = triple.predicate.get_OID();
        auto O = triple.object.get_OID();

        // see created_ids in case we need to delete something recently created
        auto found_s = created_ids.find(S);
        if (found_s != created_ids.end()) {
            S = found_s->second;
        }

        auto found_p = created_ids.find(P);
        if (found_p != created_ids.end()) {
            P = found_p->second;
        }

        auto found_o = created_ids.find(O);
        if (found_o != created_ids.end()) {
            O = found_o->second;
        }

        if (S.is_tmp() || P.is_tmp() || O.is_tmp()) {
            // tmp ids should never be saved in the database
            continue;
        }

        Record<3> record_spo = { S.id, P.id, O.id };
        bool exists = rdf_model.spo->delete_record(record_spo);

        if (exists) {
            // Retrieve all the updates that will be necessary for the text index
            if (!text_index_predicate2names.empty() || !hnsw_index_predicate2names.empty()) {
                const auto predicate_str = SPARQL::Conversions::unpack_iri(P);
                { // register text index inserts
                    const auto it = text_index_predicate2names.find(predicate_str);
                    if (it != text_index_predicate2names.end()) {
                        for (const auto& name : it->second) {
                            text_index_name2deletes[name].emplace_back(S, O);
                        }
                    }
                }
                { // register hnsw index inserts
                    const auto it = hnsw_index_predicate2names.find(predicate_str);
                    if (it != hnsw_index_predicate2names.end()) {
                        for (const auto& name : it->second) {
                            hnsw_index_name2deletes[name].emplace_back(S, O);
                        }
                    }
                }
            }

            rdf_model.catalog.delete_triple(S.id, P.id, O.id);
            graph_update_data.triples_deleted++;

            Record<3> record_pos = { P.id, O.id, S.id };
            rdf_model.pos->delete_record(record_pos);

            Record<3> record_osp = { O.id, S.id, P.id };
            rdf_model.osp->delete_record(record_osp);

            if (rdf_model.pso != nullptr) {
                Record<3> record_pso = { P.id, S.id, O.id };
                rdf_model.pso->delete_record(record_pso);
            }

            if (rdf_model.sop != nullptr) {
                Record<3> record_sop = { S.id, O.id, P.id };
                rdf_model.sop->delete_record(record_sop);
            }

            if (rdf_model.ops != nullptr) {
                Record<3> record_ops = { O.id, P.id, S.id };
                rdf_model.ops->delete_record(record_ops);
            }

            if (S == P) {
                Record<2> record_eq_sp = { S.id, O.id };
                rdf_model.equal_sp->delete_record(record_eq_sp);

                Record<2> record_eq_sp_inv = { O.id, S.id };
                rdf_model.equal_sp_inverted->delete_record(record_eq_sp_inv);

                if (P == O) {
                    Record<1> record_eq_all = { S.id };
                    rdf_model.equal_spo->delete_record(record_eq_all);
                }
            }
            if (S == O) {
                Record<2> record_eq_so = { S.id, P.id };
                rdf_model.equal_sp->delete_record(record_eq_so);

                Record<2> record_eq_so_inv = { P.id, S.id };
                rdf_model.equal_sp_inverted->delete_record(record_eq_so_inv);
            }
            if (P == O) {
                Record<2> record_eq_po = { P.id, S.id };
                rdf_model.equal_sp->delete_record(record_eq_po);

                Record<2> record_eq_po_inv = { S.id, P.id };
                rdf_model.equal_sp_inverted->delete_record(record_eq_po_inv);
            }
        }
    }

    // Execute index deletes
    for (const auto& [name, deletes] : text_index_name2deletes) {
        auto* text_search_index_ptr = rdf_model.catalog.text_index_manager.get_text_index(name);
        assert(text_search_index_ptr != nullptr && "Text index not found");

        TextIndexUpdateData hi_update {};
        hi_update.index_name = name;

        for (const auto& [S, O] : deletes) {
            const auto current_inserted_tokens = text_search_index_ptr->remove_single(S, O);
            if (current_inserted_tokens > 0) {
                ++hi_update.removed_elements;
                hi_update.inserted_tokens += current_inserted_tokens;
            }
        }

        insert_text_search_index_update_data(std::move(hi_update));
    }

    for (const auto& [name, deletes] : hnsw_index_name2deletes) {
        auto* hnsw_index_ptr = rdf_model.catalog.hnsw_index_manager.get_hnsw_index(name);
        assert(hnsw_index_ptr != nullptr && "HNSW index not found");

        HNSWIndexUpdateData hi_update {};
        hi_update.index_name = name;

        for (const auto& [S, O] : deletes) {
            const bool was_inserted = hnsw_index_ptr->remove_single(S, O);
            if (was_inserted) {
                ++hi_update.removed_elements;
            }
        }

        insert_hnsw_index_update_data(std::move(hi_update));
    }
}

void UpdateExecutor::visit(SPARQL::OpCreateTextIndex& op_create_text_index)
{
    try {
        auto& text_search_index_manager = rdf_model.catalog.text_index_manager;
        const auto& [inserted_elements, inserted_tokens] = text_search_index_manager.create_text_search_index(
            op_create_text_index.index_name,
            op_create_text_index.predicate,
            op_create_text_index.normalize_type,
            op_create_text_index.tokenize_type
        );

        TextIndexUpdateData tsi_update {};
        tsi_update.index_name = op_create_text_index.index_name;
        tsi_update.created = true;
        tsi_update.inserted_elements = inserted_elements;
        tsi_update.inserted_tokens = inserted_tokens;
        insert_text_search_index_update_data(std::move(tsi_update));
    } catch (const std::exception& e) {
        // Rethrow any exception wrapped by a QueryExecutionException
        throw QueryExecutionException(e.what());
    }
}

void UpdateExecutor::visit(SPARQL::OpCreateHNSWIndex& op_create_hnsw_index)
{
    try {
        auto& hnsw_index_manager = rdf_model.catalog.hnsw_index_manager;
        auto&& [inserted_elements] = hnsw_index_manager.create_hnsw_index<Catalog::ModelID::RDF>(
            op_create_hnsw_index.index_name,
            op_create_hnsw_index.predicate,
            op_create_hnsw_index.dimension,
            op_create_hnsw_index.max_edges,
            op_create_hnsw_index.num_candidates,
            op_create_hnsw_index.metric_type
        );

        HNSWIndexUpdateData hnsw_update {};
        hnsw_update.index_name = op_create_hnsw_index.index_name;
        hnsw_update.created = true;
        hnsw_update.inserted_elements = inserted_elements;
        insert_hnsw_index_update_data(std::move(hnsw_update));
    } catch (const std::exception& e) {
        // Rethrow any exception wrapped by a QueryExecutionException
        throw QueryExecutionException(e.what());
    }
}

void UpdateExecutor::print_stats(std::ostream& os)
{
    bool has_changes = false;

    if (!graph_update_data.empty()) {
        os << "Graph updates:\n";
        os << "  " << graph_update_data << '\n';
        has_changes = true;
    }

    if (!name2text_search_index_update_data.empty()) {
        os << "Text Index updates:\n";
        for (const auto& [_, tsi_update] : name2text_search_index_update_data) {
            os << "  " << tsi_update << '\n';
        }
        has_changes = true;
    }

    if (!name2hnsw_index_update_data.empty()) {
        os << "HNSW Index update:\n";
        for (const auto& [_, hnsw_update] : name2hnsw_index_update_data) {
            os << "  " << hnsw_update << '\n';
        }
        has_changes = true;
    }

    if (!has_changes) {
        os << "No modifications were performed\n";
        return;
    }
}

void UpdateExecutor::insert_text_search_index_update_data(TextIndexUpdateData&& data)
{
    auto it = name2text_search_index_update_data.find(data.index_name);
    if (it == name2text_search_index_update_data.end()) {
        name2text_search_index_update_data.emplace(data.index_name, std::move(data));
    } else {
        it->second.inserted_elements += data.inserted_elements;
        it->second.inserted_tokens += data.inserted_tokens;
        it->second.removed_elements += data.removed_elements;
        it->second.removed_tokens += data.removed_tokens;
    }
}

void UpdateExecutor::insert_hnsw_index_update_data(HNSWIndexUpdateData&& data)
{
    auto it = name2hnsw_index_update_data.find(data.index_name);
    if (it == name2hnsw_index_update_data.end()) {
        name2hnsw_index_update_data.emplace(data.index_name, std::move(data));
    } else {
        it->second.inserted_elements += data.inserted_elements;
    }
}
