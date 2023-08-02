#pragma once

#include <cstdint>
#include <string>

#include "char_iter.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "storage/string_manager.h"
#include "storage/tmp_manager.h"

class UnifiedStorage {
public:
    // Used for external and temporary strings, not inline strings

    /**
     * @brief First looks up the str in the string_manager and if found returns the id
     * masked with the subtype_mask and MOD_EXTERNAL. If not found the tmp_manager is checked,
     * which creates str if it does not already exist. The id received from the tmp_manager
     * is masked with subtype_mask and MOD_TMP and returned.
     *
     * @param str the string to look up in string_manager and tmp_manager.
     * @param subtype_mask the subtype mask to apply to the id before returning.
     * @return an uint64_t id that can be converted to ObjectId.
     */
    static inline uint64_t get_str_id(const std::string& str, uint64_t subtype_mask) {
        auto str_id = string_manager.get_str_id(str);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            return subtype_mask | ObjectId::MOD_EXTERNAL | str_id;
        } else {
            return subtype_mask | ObjectId::MOD_TMP | tmp_manager.get_str_id(str);
        }
    }

    /**
     * @brief First looks up the str in the string_manager and if found returns the id
     * masked with the subtype_mask and MOD_EXTERNAL. If not found the tmp_manager is checked,
     * which creates str if it does not already exist. The id received from the tmp_manager
     * is masked with subtype_mask and MOD_TMP and returned.
     *
     * @param str the string to look up in string_manager and tmp_manager.
     * @param subtype_mask the subtype mask to apply to the id before returning.
     * @return an uint64_t id that can be converted to ObjectId.
     */
    static inline uint64_t get_bytes_id(const char* bytes, uint64_t size, uint64_t subtype_mask) {
        auto bytes_id = string_manager.get_bytes_id(bytes, size);
        if (bytes_id != ObjectId::MASK_NOT_FOUND) {
            return subtype_mask | ObjectId::MOD_EXTERNAL | bytes_id;
        } else {
            return subtype_mask | ObjectId::MOD_TMP | tmp_manager.get_bytes_id(bytes, size);
        }
    }

    /**
     * @brief First looks up the datatype in the datatype catalog and if found returns
     * the id (index). If not found, the tmp_manager is checked,
     * which creates the datatype if it does not already exist. The id received from the
     * tmp_manager has it's most significant bit (bit 15) set and is returned.
     *
     * @param str the datatype to look up in string_manager and tmp_manager.
     * @return an uint64_t containing the id (unshifted) in the two least significant bytes.
     */
    static inline uint64_t get_datatype_id(const std::string& str) {
        const auto& datatypes = rdf_model.catalog().datatypes;
        auto        it        = std::find(datatypes.cbegin(), datatypes.cend(), str);
        if (it != datatypes.end()) {
            return (it - datatypes.cbegin());
        } else {
            return tmp_manager.get_dtt_id(str) | (ObjectId::MASK_TAG_MANAGER);
        }
    }

    /**
     * @brief First looks up the language in the datatype catalog and if found returns
     * the id (index). If not found, the tmp_manager is checked,
     * which creates the language if it does not already exist. The id received from the
     * tmp_manager has it's most significant bit (bit 15) set and is returned.
     *
     * @param str the language to look up in string_manager and tmp_manager.
     * @return an uint64_t containing the id (unshifted) in the two least significant bytes.
     */
    static inline uint64_t get_language_id(const std::string& str) {
        const auto& languages = rdf_model.catalog().languages;
        auto        it        = std::find(languages.cbegin(), languages.cend(), str);
        if (it != languages.end()) {
            return (it - languages.cbegin());
        } else {
            return tmp_manager.get_lan_id(str) | (ObjectId::MASK_TAG_MANAGER);
        }
    }

    /**
     * @brief Returns a CharIter of the external/temporary string referenced by oid.
     *
     * @param oid an ObjectId containing an external/temporary iri, string or decimal.
     * @return a CharIter of the referenced string, not including datatypes or languages.
     */
    static inline std::unique_ptr<CharIter> get_str_char_iter(ObjectId oid) {
        uint64_t external_id_mask;
        switch (oid.get_sub_type()) {
        case ObjectId::MASK_IRI: {
            external_id_mask = ObjectId::MASK_IRI_CONTENT;
            break;
        }
        case ObjectId::MASK_STRING_SIMPLE:
        case ObjectId::MASK_STRING_XSD: {
            external_id_mask = ObjectId::VALUE_MASK;
            break;
        }
        case ObjectId::MASK_STRING_DATATYPE:
        case ObjectId::MASK_STRING_LANG: {
            external_id_mask = ObjectId::MASK_LITERAL;
            break;
        }
        case ObjectId::MASK_DECIMAL: {
            external_id_mask = ObjectId::VALUE_MASK;
            break;
        }
        default:
            throw LogicException("Incorrectly called print_str");
        }

        uint64_t external_id = oid.id & external_id_mask;
        switch (oid.get_mod()) {
        case ObjectId::MOD_TMP: {
            return tmp_manager.get_str_char_iter(external_id);
        }
        case ObjectId::MOD_EXTERNAL: {
            return string_manager.get_char_iter(external_id);
        }
        default:
            throw LogicException("Incorrectly called print_str");
        }
    }

    /**
     * @brief Returns a CharIter of the string representation of the datatype with identifier id.
     *
     * @param id the unshifted identifier of the datatype.
     * @return a CharIter of the string representation of the datatype.
     */
    static inline std::unique_ptr<CharIter> get_datatype_char_iter(uint64_t id) {
        if (id >= ObjectId::MASK_TAG_MANAGER) {
            return tmp_manager.get_datatype_char_iter(id & (~ObjectId::MASK_TAG_MANAGER));
        } else {
            const auto& str = rdf_model.catalog().datatypes[id];
            return std::make_unique<StringTmpIter>(str);
        }
    }

    /**
     * @brief Returns a CharIter of the string representation of the language with identifier id.
     *
     * @param id the unshifted identifier of the language.
     * @return a CharIter of the string representation of the language.
     */
    static inline std::unique_ptr<CharIter> get_language_char_iter(uint64_t id) {
        if (id >= ObjectId::MASK_TAG_MANAGER) {
            return tmp_manager.get_language_char_iter(id & (~ObjectId::MASK_TAG_MANAGER));
        } else {
            const auto& str = rdf_model.catalog().languages[id];
            return std::make_unique<StringTmpIter>(str);
        }
    }

    /**
     * @brief Writes the external/temporary string referenced by oid to the ostream os.
     *
     * @param os the ostream to write the string to.
     * @param oid an ObjectId containing an external/temporary iri, string or decimal.
     */
    static inline void print_str(std::ostream& os, ObjectId oid) {
        uint64_t external_id_mask;
        switch (oid.get_sub_type()) {
        case ObjectId::MASK_IRI: {
            external_id_mask = ObjectId::MASK_IRI_CONTENT;
            break;
        }
        case ObjectId::MASK_STRING_DATATYPE:
        case ObjectId::MASK_STRING_LANG: {
            external_id_mask = ObjectId::MASK_LITERAL;
            break;
        }
        case ObjectId::MASK_NAMED_NODE:
        case ObjectId::MASK_STRING_XSD:
        case ObjectId::MASK_STRING_SIMPLE:
        case ObjectId::MASK_DOUBLE:
        case ObjectId::MASK_DECIMAL: {
            external_id_mask = ObjectId::VALUE_MASK;
            break;
        }
        default:
            throw LogicException("Incorrectly called print_str");
        }

        uint64_t external_id = oid.id & external_id_mask;
        switch (oid.get_mod()) {
        case ObjectId::MOD_TMP: {
            tmp_manager.print_str(os, external_id);
            break;
        }
        case ObjectId::MOD_EXTERNAL: {
            string_manager.print(os, external_id);
            break;
        }
        default:
            throw LogicException("Incorrectly called print_str");
        }
    }

    /**
     * @brief Writes the string representation of the language referenced by id to the ostream os.
     *
     * @param os the ostream to write the string to.
     * @param id the unshifted identifier of the language.
     */
    static inline void print_language(std::ostream& os, uint64_t id) {
        if (id >= ObjectId::MASK_TAG_MANAGER) {
            tmp_manager.print_lan(os, id & (~ObjectId::MASK_TAG_MANAGER));
        } else {
            os << rdf_model.catalog().languages[id];
        }
    }

    /**
     * @brief Writes the string representation of the datatype referenced by id to the ostream os.
     *
     * @param os the ostream to write the string to.
     * @param id the unshifted identifier of the datatype.
     */
    static inline void print_datatype(std::ostream& os, uint64_t id) {
        if (id >= ObjectId::MASK_TAG_MANAGER) {
            tmp_manager.print_dtt(os, id & (~ObjectId::MASK_TAG_MANAGER));
        } else {
            os << rdf_model.catalog().datatypes[id];
        }
    }
};
