#include "update_executor.h"

#include <sstream>

#include "graph_models/inliner.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

using namespace SPARQL;

UpdateExecutor::~UpdateExecutor() {
    // TODO: force string file WAL flush?
}


constexpr uint64_t CLEAR_TMP_MASK = ~(ObjectId::MOD_MASK | ObjectId::MASK_EXTERNAL_ID);
constexpr uint64_t CLEAR_TAG_MASK = ~(ObjectId::MASK_LITERAL_TAG);


// helper to generalize inline langs and inline datatypes
bool UpdateExecutor::try_transform_inline(ObjectId& oid, std::vector<std::string>& catalog_list, char split) {
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
        throw LogicException("try_transform_inline: string with LAST_TMP_ID `" + tmp_str + "` must have " + split + " as separator");
    }
    std::string new_str = tmp_str.substr(0,found);
    std::string suffix = tmp_str.substr(found+1);

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


bool UpdateExecutor::try_transform_tmp(ObjectId& oid, std::vector<std::string>& catalog_list, char split) {
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
        throw LogicException("try_transform_tmp: string with LAST_TMP_ID `" + tmp_str + "` must have " + split + " as separator");
    }
    std::string new_str = tmp_str.substr(0,found);
    std::string suffix = tmp_str.substr(found+1);

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


bool UpdateExecutor::try_transform_extern(ObjectId& oid, std::vector<std::string>& catalog_list, char split) {
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
        throw LogicException("try_transform_extern string with LAST_TMP_ID `" + old_str + "` must have " + split + " as separator");
    }
    std::string new_str = old_str.substr(0,found);
    std::string suffix = old_str.substr(found+1);

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
bool UpdateExecutor::transform_if_tmp(ObjectId& oid) {
    auto oid_type = RDF_OID::get_type(oid);

    switch (oid_type) {
    case RDF_OID::Type::STRING_LANG_INLINE: {
        return try_transform_inline(oid, rdf_model.catalog().languages, '@');
    }
    case RDF_OID::Type::STRING_LANG_EXTERN: {
        return try_transform_extern(oid, rdf_model.catalog().languages, '@');
    }
    case RDF_OID::Type::STRING_LANG_TMP: {
        return try_transform_tmp(oid, rdf_model.catalog().languages, '@');
    }
    case RDF_OID::Type::STRING_DATATYPE_INLINE: {
        return try_transform_inline(oid, rdf_model.catalog().datatypes, '^');
    }
    case RDF_OID::Type::STRING_DATATYPE_EXTERN: {
        return try_transform_extern(oid, rdf_model.catalog().datatypes, '^');
    }
    case RDF_OID::Type::STRING_DATATYPE_TMP: {
        return try_transform_tmp(oid, rdf_model.catalog().datatypes, '^');
    }
    default: {
        if (oid.is_tmp()) {
            // we need to materialize the string
            auto tmp_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
            auto& str = tmp_manager.get_str(tmp_id);
            auto new_external_id = string_manager.get_or_create(str.data(), str.size());

            oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_EXTERNAL | new_external_id;
        }
        return true;
    }
    }
}


void UpdateExecutor::visit(SPARQL::OpInsertData& op_insert_data) {
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
            created_ids.insert({originalS, S});
        }
        if (transform_if_tmp(P)) {
            created_ids.insert({originalP, P});
        }
        if (transform_if_tmp(O)) {
            created_ids.insert({originalO, O});
        }

        assert(!S.is_tmp());
        assert(!P.is_tmp());
        assert(!O.is_tmp());

        Record<3> record_spo = { S.id, P.id, O.id };
        bool is_new_record = rdf_model.spo->insert(record_spo);

        if (is_new_record) {
            rdf_model.catalog().insert_triple(S.id, P.id, O.id);
            triples_inserted++;

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
}


void UpdateExecutor::visit(SPARQL::OpDeleteData& op_delete_data) {
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
            rdf_model.catalog().delete_triple(S.id, P.id, O.id);
            triples_deleted++;

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
}
