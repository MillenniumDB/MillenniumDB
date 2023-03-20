#include "sparql_element_to_object_id.h"

#include "base/exceptions.h"
#include "query_optimizer/rdf_model/rdf_model.h"
#include "storage/string_manager.h"

ObjectId SparqlElementToObjectId::operator()(const SparqlElement& elt) {
    return std::visit(*this, elt.value);
}


ObjectId SparqlElementToObjectId::get_string_id(const std::string& str, uint64_t mask_inlined, uint64_t mask_external) {
    if (str.size() < 8) {
        uint64_t res = 0;
        int shift_size = 8*6;
        // MUST convert to uint8_t and then to uint64_t.
        // Shift with shift_size >=32 is undefined behaviour.
        for (uint8_t byte : str) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | mask_inlined);
    } else {
        return ObjectId(string_manager.get_str_id(str, create_if_not_exists) | mask_external);
    }
}


ObjectId SparqlElementToObjectId::operator()(const Var&) {
    throw LogicException("Var cannot be converted into ObjectId");
}


ObjectId SparqlElementToObjectId::operator()(const Iri& iri) {
    auto& prefixes = rdf_model.catalog().prefixes;
    std::string str = iri.name;
    uint8_t prefix_id = 0;
    for (size_t i = 0; i < prefixes.size(); ++i) {
        if (str.compare(0, prefixes[i].size(), prefixes[i]) == 0) {
            str = str.substr(prefixes[i].size(), str.size() - prefixes[i].size());
            prefix_id = i;
            break;
        }
    }

    uint64_t shifted_prefix_id = static_cast<uint64_t>(prefix_id) << 48;
    if (str.size() < 7) {
        uint64_t res = 0;
        int shift_size = 8 * 5;
        for (uint8_t byte : str) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | ObjectId::MASK_IRI_INLINED | shifted_prefix_id);
    } else {
        uint64_t external_id = string_manager.get_str_id(str, create_if_not_exists);
        if (external_id == ObjectId::OBJECT_ID_NOT_FOUND) {
            return ObjectId::get_not_found();
        } else {
            return ObjectId(external_id | ObjectId::MASK_IRI_EXTERN | shifted_prefix_id);
        }
    }
}


ObjectId SparqlElementToObjectId::operator()(const Literal& l) {
    return get_string_id(l.str, ObjectId::MASK_STRING_INLINED, ObjectId::MASK_STRING_EXTERN);
}


ObjectId SparqlElementToObjectId::operator()(const LiteralDatatype& ld) {
    auto& datatypes = rdf_model.catalog().datatypes;

    uint16_t datatype_id = 0;
    bool found           = false;
    for (size_t i = 0; i < datatypes.size(); ++i) {
        if (ld.datatype == datatypes[i]) {
            datatype_id = i;
            found       = true;
            break;
        }
    }
    // TODO: implement handle datatype_id not found ? check this
    if (!found) {
        return ObjectId::get_not_found();
    }

    uint64_t shifted_datatype_id = static_cast<uint64_t>(datatype_id) << 40;
    if (ld.str.size() < 6) {
        uint64_t res = 0;
        int shift_size = 8*4;
        for (uint8_t byte : ld.str) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | ObjectId::MASK_STRING_DATATYPE_INLINED | shifted_datatype_id);
    } else {
        uint64_t external_id = string_manager.get_str_id(ld.str, create_if_not_exists);
        if (external_id == ObjectId::OBJECT_ID_NOT_FOUND) {
            return ObjectId::get_not_found();
        } else {
            return ObjectId(external_id | ObjectId::MASK_STRING_DATATYPE_EXTERN | shifted_datatype_id);
        }
    }
}


ObjectId SparqlElementToObjectId::operator()(const LiteralLanguage& ll) {
    auto& languages = rdf_model.catalog().languages;

    uint16_t language_id = 0;
    bool found           = false;
    for (size_t i = 0; i < languages.size(); ++i) {
        if (ll.language == languages[i]) {
            language_id = i;
            found       = true;
            break;
        }
    }
    // TODO: implement handle language_id not found ? check this
    if (!found) {
        return ObjectId::get_not_found();
    }

    uint64_t shifted_language_id = static_cast<uint64_t>(language_id) << 40;
    if (ll.str.size() < 6) {
        uint64_t res = 0;
        int shift_size = 8*4;
        for (uint8_t byte : ll.str) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | ObjectId::MASK_STRING_LANG_INLINED | shifted_language_id);
    } else {
        uint64_t external_id = string_manager.get_str_id(ll.str, create_if_not_exists);
        if (external_id == ObjectId::OBJECT_ID_NOT_FOUND) {
            return ObjectId::get_not_found();
        } else {
            return ObjectId(external_id | ObjectId::MASK_STRING_LANG_EXTERN | shifted_language_id);
        }
    }
}

ObjectId SparqlElementToObjectId::operator()(DateTime dt) {
    return ObjectId(dt.id | ObjectId::MASK_DATETIME);
}

ObjectId SparqlElementToObjectId::operator()(Decimal dec) {
    uint64_t decimal_id = DecimalInlined::get_decimal_id(dec.str.c_str());
    if (decimal_id == DecimalInlined::INVALID_ID) {
        uint64_t external_id = string_manager.get_str_id(Decimal::normalize(dec.str), create_if_not_exists);
        if (external_id == ObjectId::OBJECT_ID_NOT_FOUND) {
            return ObjectId::get_not_found();
        } else {
            return ObjectId(external_id | ObjectId::MASK_DECIMAL_EXTERN);
        }
    }
    else {
        return ObjectId(decimal_id | ObjectId::MASK_DECIMAL_INLINED);
    }
}

ObjectId SparqlElementToObjectId::operator()(bool b) {
    return ObjectId(ObjectId::MASK_BOOL | (b ? 0x01 : 0x00));
}

ObjectId SparqlElementToObjectId::operator()(const std::unique_ptr<SPARQL::IPath>&) {
    throw LogicException("IPath cannot be converted into ObjectId");
}
