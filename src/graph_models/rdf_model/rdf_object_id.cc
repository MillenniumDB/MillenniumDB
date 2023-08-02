#include "rdf_object_id.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "storage/unified_storage.h"

ObjectId RDFObjectId::get_string_id(const std::string& str, uint64_t subtype_mask) {
    if (str.size() <= ObjectId::STR_INLINE_BYTES) {
        uint64_t res = 0;
        int shift_size = 8*(ObjectId::STR_INLINE_BYTES-1);
        // MUST convert to uint8_t and then to uint64_t.
        // Shift with shift_size >=32 is undefined behaviour.
        for (uint8_t byte : str) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | subtype_mask | ObjectId::MOD_INLINE);
    } else {
        return ObjectId(UnifiedStorage::get_str_id(str, subtype_mask));
    }
}


ObjectId RDFObjectId::get_Iri(const std::string& iri) {
    auto& prefixes = rdf_model.catalog().prefixes;
    std::string str = iri; // need to create a copy
    uint8_t prefix_id = 0;
    for (size_t i = 1; i < prefixes.size(); ++i) {
        if (str.compare(0, prefixes[i].size(), prefixes[i]) == 0) {
            str = str.substr(prefixes[i].size(), str.size() - prefixes[i].size());
            prefix_id = i;
            break;
        }
    }

    uint64_t shifted_prefix_id = static_cast<uint64_t>(prefix_id) << 48;
    if (str.size() <= ObjectId::IRI_INLINE_BYTES) {
        uint64_t res = 0;
        int shift_size = 8 * (ObjectId::IRI_INLINE_BYTES-1);
        for (uint8_t byte : str) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | ObjectId::MASK_IRI_INLINED | shifted_prefix_id);
    } else {
        uint64_t external_id = UnifiedStorage::get_str_id(str, ObjectId::MASK_IRI);
        return ObjectId(external_id | shifted_prefix_id);
    }
}


ObjectId RDFObjectId::get_Literal(const std::string& l) {
    return get_string_id(l, ObjectId::MASK_STRING_SIMPLE);
}


ObjectId RDFObjectId::get_LiteralXSDString(const std::string& l) {
    return get_string_id(l, ObjectId::MASK_STRING_XSD);
}


ObjectId RDFObjectId::get_LiteralDatatype(const std::string& literal, const std::string& datatype) {
    uint64_t datatype_id = UnifiedStorage::get_datatype_id(datatype);

    uint64_t shifted_datatype_id = static_cast<uint64_t>(datatype_id) << 40;
    if (literal.size() <= ObjectId::STR_DT_INLINE_BYTES) {
        uint64_t res = 0;
        int shift_size = 8*(ObjectId::STR_DT_INLINE_BYTES-1);
        for (uint8_t byte : literal) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | ObjectId::MASK_STRING_DATATYPE_INLINED | shifted_datatype_id);
    } else {
        uint64_t external_id = UnifiedStorage::get_str_id(literal, ObjectId::MASK_STRING_DATATYPE);
        return ObjectId(external_id | shifted_datatype_id);
    }
}


ObjectId RDFObjectId::get_LiteralLang(const std::string& literal, const std::string& lang) {
    uint64_t language_id = UnifiedStorage::get_language_id(lang);

    uint64_t shifted_language_id = static_cast<uint64_t>(language_id) << 40;
    if (literal.size() <= ObjectId::STR_LANG_INLINE_BYTES) {
        uint64_t res = 0;
        int shift_size = 8*(ObjectId::STR_LANG_INLINE_BYTES-1);
        for (uint8_t byte : literal) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | ObjectId::MASK_STRING_LANG_INLINED | shifted_language_id);
    } else {
        uint64_t external_id = UnifiedStorage::get_str_id(literal, ObjectId::MASK_STRING_LANG);
        return ObjectId(external_id | shifted_language_id);
    }
}


ObjectId RDFObjectId::get(DateTime dt) {
    return ObjectId(dt.id);
}


ObjectId RDFObjectId::get(Decimal dec) {
    auto oid = dec.to_internal();
    if (oid == ObjectId::NULL_ID) {
        auto bytes = dec.to_bytes();
        oid = UnifiedStorage::get_bytes_id(reinterpret_cast<const char*>(bytes.data()),
                                           bytes.size(),
                                           ObjectId::MASK_DECIMAL);
    }
    return ObjectId(oid);
}


ObjectId RDFObjectId::get(bool b) {
    return ObjectId(ObjectId::MASK_BOOL | (b ? 0x01 : 0x00));
}


ObjectId RDFObjectId::get(int64_t i) {
    // If the integer uses more than 56 bits, it must be converted into Decimal Extern (overflow)
    if (i < -0x00FF'FFFF'FFFF'FFFF || i > 0x00FF'FFFF'FFFF'FFFF) {
        auto bytes = Decimal(i).to_bytes();
        uint64_t external_id = UnifiedStorage::get_bytes_id(reinterpret_cast<const char*>(bytes.data()),
                                                            bytes.size(),
                                                            ObjectId::MASK_DECIMAL);
        return ObjectId(external_id);
    } else if (i < 0) {
        i *= -1;
        i = (~i) & 0x00FF'FFFF'FFFF'FFFFUL;
        return ObjectId(i | ObjectId::MASK_NEGATIVE_INT);
    } else {
        return ObjectId(i | ObjectId::MASK_POSITIVE_INT);
    }
}


ObjectId RDFObjectId::get(float f) {
    static_assert(sizeof(f) == 4);
    unsigned char bytes[sizeof(f)];
    memcpy(bytes, &f, sizeof(f));

    uint64_t res = 0;
    int shift_size = 0;
    for (std::size_t i = 0; i < sizeof(bytes); ++i) {
        uint64_t byte = bytes[i];
        res |= byte << shift_size;
        shift_size += 8;
    }
    return ObjectId(ObjectId::MASK_FLOAT | res);
}


ObjectId RDFObjectId::get(double d) {
    return SPARQL::Conversions::pack_double(d);
}
