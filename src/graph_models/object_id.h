#pragma once

#include <cstdint>
#include <ostream>
#include <type_traits>

enum class ObjectGenType {
    Null,
    Bool,
    TemporalLiteral,
    String,
    Numeric,
    Tensor,
    List,
    Dict,

    // MQL
    Anon,
    NamedNode,
    Edge,
    Path,

    // RDF
    Iri,

    NotFound
};


enum class ObjectSubType {
    Null,
    Bool,
    Int,
    Float,
    Double,
    Decimal,
    TemporalLiteral, // Time, Date, DateTime, DateTimestamp,
    String,
    StringXsd,
    StringLang,
    StringDatatype,
    TensorFloat,
    TensorDouble,
    Dictionary,
    List,

    // MQL
    Anon,
    NamedNode,
    Edge,
    Path,

    // RDF
    Iri,

    NotFound
};

enum class ObjectType {
    Null,

    AnonInl, // also represents blank inlined in RDF, or node in GQL
    AnonTmp,

    Bool,
    NegativeInt56,
    PositiveInt56,
    Float,

    DoubleExt,
    DoubleTmp,

    DecimalInl,
    DecimalExt,
    DecimalTmp,

    StringInl,
    StringExt,
    StringTmp,

    StringXsdInl,
    StringXsdExt,
    StringXsdTmp,

    StringLangInl,
    StringLangExt,
    StringLangTmp,

    StringDatatypeInl,
    StringDatatypeExt,
    StringDatatypeTmp,

    IriInl,
    IriExt,
    IriTmp,

    IriUuidLowerTmp,
    IriUuidLowerExt,
    IriUuidUpperTmp,
    IriUuidUpperExt,

    IriHexLowerTmp,
    IriHexLowerExt,
    IriHexUpperTmp,
    IriHexUpperExt,

    NamedNodeInl,
    NamedNodeExt,
    NamedNodeTmp,

    Time,
    Date,
    Datetime,
    Datetimestamp,

    TensorFloatExt,
    TensorFloatTmp,

    TensorDoubleExt,
    TensorDoubleTmp,

    DictionaryExt,
    DictionaryTmp,

    ListExt,
    ListTmp,

    DirectedEdge,
    UndirectedEdge,
    Path,

    EdgeKey, // Only For GQL
    NodeKey, // Only For GQL
    EdgeLabel, // Only For GQL
    NodeLabel, // Only For GQL

    NotFound,
};

std::string to_string(ObjectType type);
std::string to_string(ObjectSubType type);
std::string to_string(ObjectGenType type);

class ObjectId {
public:
    static constexpr int MAX_LEN_INLINE_STRING = 7;
    static constexpr int MAX_LEN_INLINE_STRING_DATATYPE = 5;
    static constexpr int MAX_LEN_INLINE_STRING_LANG = 5;
    static constexpr int MAX_LEN_INLINE_IRI = 6;

    static constexpr uint64_t VALUE_MASK        = 0x00'FFFFFFFFFFFFFFUL;
    static constexpr uint64_t TYPE_MASK         = 0xFF'00000000000000UL; // 0b1111'11'11

    static constexpr uint64_t MOD_MASK          = 0x03'00000000000000UL; // 0b0000'00'11
    static constexpr uint64_t MOD_INLINE        = 0x00'00000000000000UL; // 0b0000'00'00
    static constexpr uint64_t MOD_EXTERNAL      = 0x01'00000000000000UL; // 0b0000'00'01
    static constexpr uint64_t MOD_TMP           = 0x02'00000000000000UL; // 0b0000'00'10

    static constexpr uint64_t MASK_LITERAL_TAG  = 0x00'FFF00000000000UL; // 12 bits for language/datatype of string
    static constexpr uint64_t MASK_EXTERNAL_ID  = 0x00'000FFFFFFFFFFFUL; // 44 least significant bits for external/tmp id

    static constexpr uint64_t MASK_IRI_PREFIX   = 0x00'FF000000000000UL;
    static constexpr uint64_t MASK_IRI_CONTENT  = 0x00'00FFFFFFFFFFFFUL;

    static constexpr int IRI_INLINE_BYTES        = 6; // Number of bytes of IRI, excluding prefix id, stored inline
    static constexpr int STR_INLINE_BYTES        = 7; // Number of bytes of string stored inline
    static constexpr int NAMED_NODE_INLINE_BYTES = 7; // Number of bytes of named node stored inline
    static constexpr int STR_DT_INLINE_BYTES     = 5; // Number of bytes of string, excluding datatype id, stored inline
    static constexpr int STR_LANG_INLINE_BYTES   = 5; // Number of bytes of string, excluding language id, stored inline


    static constexpr uint64_t MASK_NULL               = 0x00'00000000000000UL; // 0b000000'00
    static constexpr uint64_t MASK_ANON_INL           = 0x10'00000000000000UL; // 0b000100'00
    static constexpr uint64_t MASK_ANON_TMP           = 0x12'00000000000000UL; // 0b000100'10
    static constexpr uint64_t MASK_NAMED_NODE_INL     = 0x20'00000000000000UL; // 0b001000'00
    static constexpr uint64_t MASK_NAMED_NODE_EXT     = 0x21'00000000000000UL; // 0b001000'01
    static constexpr uint64_t MASK_NAMED_NODE_TMP     = 0x22'00000000000000UL; // 0b001000'10
    static constexpr uint64_t MASK_IRI_INL            = 0x30'00000000000000UL; // 0b001100'00
    static constexpr uint64_t MASK_IRI_EXT            = 0x31'00000000000000UL; // 0b001100'01
    static constexpr uint64_t MASK_IRI_TMP            = 0x32'00000000000000UL; // 0b001100'10
    static constexpr uint64_t MASK_DIRECTED_EDGE      = 0x80'00000000000000UL; // 0b111000'00
    static constexpr uint64_t MASK_IRI_UUID_LOWER_EXT = 0xA1'00000000000000UL; // 0b101000'01
    static constexpr uint64_t MASK_IRI_UUID_LOWER_TMP = 0xA2'00000000000000UL; // 0b101000'10
    static constexpr uint64_t MASK_IRI_UUID_UPPER_EXT = 0xA5'00000000000000UL; // 0b101001'01
    static constexpr uint64_t MASK_IRI_UUID_UPPER_TMP = 0xA6'00000000000000UL; // 0b101001'10
    static constexpr uint64_t MASK_IRI_HEX_LOWER_EXT  = 0xA9'00000000000000UL; // 0b101010'01
    static constexpr uint64_t MASK_IRI_HEX_LOWER_TMP  = 0xAA'00000000000000UL; // 0b101010'10
    static constexpr uint64_t MASK_IRI_HEX_UPPER_EXT  = 0xAD'00000000000000UL; // 0b101011'01
    static constexpr uint64_t MASK_IRI_HEX_UPPER_TMP  = 0xAE'00000000000000UL; // 0b101011'10
    static constexpr uint64_t MASK_UNDIRECTED_EDGE    = 0xE4'00000000000000UL; // 0b111001'00
    static constexpr uint64_t MASK_NODE_LABEL         = 0xE8'00000000000000UL; // 0b111010'00
    static constexpr uint64_t MASK_EDGE_LABEL         = 0xEC'00000000000000UL; // 0b111011'00
    static constexpr uint64_t MASK_NODE_KEY           = 0xF0'00000000000000UL; // 0b111100'00
    static constexpr uint64_t MASK_EDGE_KEY           = 0xF4'00000000000000UL; // 0b111101'00
    static constexpr uint64_t MASK_DIRECTION          = 0xFC'00000000000000UL; // 0b111111'00
    static constexpr uint64_t MASK_NOT_FOUND          = 0xFF'00000000000000UL; // 0b111111'11


    // TODO: new named node types
    // static constexpr uint64_t MASK_NAMED_NODE_HEX_EXTERN   = 0x25'00000000000000UL;
    // static constexpr uint64_t MASK_NAMED_NODE_HEX_TMP      = 0x26'00000000000000UL;

    static constexpr uint64_t MASK_STR_INL               = 0x40'00000000000000UL; // 0b010000'00
    static constexpr uint64_t MASK_STR_EXT               = 0x41'00000000000000UL; // 0b010000'01
    static constexpr uint64_t MASK_STR_TMP               = 0x42'00000000000000UL; // 0b010000'10
    static constexpr uint64_t MASK_STR_XSD_INL           = 0x44'00000000000000UL; // 0b010001'00
    static constexpr uint64_t MASK_STR_XSD_EXT           = 0x45'00000000000000UL; // 0b010001'01
    static constexpr uint64_t MASK_STR_XSD_TMP           = 0x46'00000000000000UL; // 0b010001'10
    static constexpr uint64_t MASK_STR_LANG_INL          = 0x48'00000000000000UL; // 0b010010'00
    static constexpr uint64_t MASK_STR_LANG_EXT          = 0x49'00000000000000UL; // 0b010010'01
    static constexpr uint64_t MASK_STR_LANG_TMP          = 0x4A'00000000000000UL; // 0b010010'10
    static constexpr uint64_t MASK_STR_DATATYPE_INL      = 0x4C'00000000000000UL; // 0b010011'00
    static constexpr uint64_t MASK_STR_DATATYPE_EXT      = 0x4D'00000000000000UL; // 0b010011'01
    static constexpr uint64_t MASK_STR_DATATYPE_TMP      = 0x4E'00000000000000UL; // 0b010011'10

    static constexpr uint64_t MASK_NEGATIVE_INT          = 0x50'00000000000000UL; // 0b010100'00
    static constexpr uint64_t MASK_POSITIVE_INT          = 0x51'00000000000000UL; // 0b010100'01
    static constexpr uint64_t MASK_DECIMAL_INL           = 0x54'00000000000000UL; // 0b010101'00
    static constexpr uint64_t MASK_DECIMAL_EXT           = 0x55'00000000000000UL; // 0b010101'01
    static constexpr uint64_t MASK_DECIMAL_TMP           = 0x56'00000000000000UL; // 0b010101'10
    static constexpr uint64_t MASK_FLOAT                 = 0x58'00000000000000UL; // 0b010110'00
    static constexpr uint64_t MASK_DOUBLE_EXT            = 0x5D'00000000000000UL; // 0b010111'01
    static constexpr uint64_t MASK_DOUBLE_TMP            = 0x5E'00000000000000UL; // 0b010111'10

    static constexpr uint64_t MASK_DT_DATE               = 0x60'00000000000000UL; // 0b011000'00
    static constexpr uint64_t MASK_DT_TIME               = 0x64'00000000000000UL; // 0b011001'00
    static constexpr uint64_t MASK_DT_DATETIME           = 0x68'00000000000000UL; // 0b011010'00
    static constexpr uint64_t MASK_DT_DATETIMESTAMP      = 0x6C'00000000000000UL; // 0b011011'00

    static constexpr uint64_t MASK_BOOL                  = 0x70'00000000000000UL; // 0b011100'00
    static constexpr uint64_t MASK_PATH                  = 0x90'00000000000000UL; // 0b100100'00

    // Inlined tensors are only used to represent the empty tensor
    // TODO: delete inlined tensors?
    // static constexpr uint64_t MASK_TENSOR_FLOAT_INL      = 0xB0'00000000000000UL; // 0b101100'00
    static constexpr uint64_t MASK_TENSOR_FLOAT_EXT      = 0xB1'00000000000000UL; // 0b101100'01
    static constexpr uint64_t MASK_TENSOR_FLOAT_TMP      = 0xB2'00000000000000UL; // 0b101100'10
    // static constexpr uint64_t MASK_TENSOR_DOUBLE_INL     = 0xB4'00000000000000UL; // 0b101110'00
    static constexpr uint64_t MASK_TENSOR_DOUBLE_EXT     = 0xB5'00000000000000UL; // 0b101110'01
    static constexpr uint64_t MASK_TENSOR_DOUBLE_TMP     = 0xB6'00000000000000UL; // 0b101100'10

    static constexpr uint64_t MASK_LIST_EXT              = 0xC1'00000000000000UL; // 0b110000'01
    static constexpr uint64_t MASK_LIST_TMP              = 0xC2'00000000000000UL; // 0b110000'10
    static constexpr uint64_t MASK_DICTIONARY_EXT        = 0xD1'00000000000000UL; // 0b110100'01
    static constexpr uint64_t MASK_DICTIONARY_TMP        = 0xD2'00000000000000UL; // 0b110100'10





    static constexpr uint64_t DIRECTION_RIGHT       = MASK_DIRECTION | 0x0UL;
    static constexpr uint64_t DIRECTION_LEFT        = MASK_DIRECTION | 0x1UL;
    static constexpr uint64_t DIRECTION_UNDIRECTED  = MASK_DIRECTION | 0x2UL;

    static_assert(MASK_NEGATIVE_INT < MASK_POSITIVE_INT, "Integers won't be ordered properly in the B+Tree.");
    static_assert(MASK_NEGATIVE_INT < 0x80'00000000000000UL, "Integer IDs can't be subtracted without overflow.");
    static_assert(MASK_POSITIVE_INT < 0x80'00000000000000UL, "Integer IDs can't be subtracted without overflow.");

    static constexpr uint64_t NULL_ID    = MASK_NULL;
    static constexpr uint64_t BOOL_FALSE = MASK_BOOL | 0UL;
    static constexpr uint64_t BOOL_TRUE  = MASK_BOOL | 1UL;

    uint64_t id;

    explicit constexpr ObjectId(uint64_t id) : id(id) { }

    ObjectId() = default;

    static constexpr ObjectId get_null() noexcept {
        return ObjectId(NULL_ID);
    }

    static constexpr ObjectId get_not_found() noexcept {
        return ObjectId(MASK_NOT_FOUND);
    }

    inline ObjectType type() const noexcept {
        // TODO:
        return ObjectType::NotFound;
        // return id & TYPE_MASK;
    }

    inline ObjectGenType generic_type() const noexcept {
        // return id & SUB_TYPE_MASK;
        // static constexpr uint64_t SUB_TYPE_MASK = 0xFC'00000000000000UL; // 0b1111'11'00
        // TODO:
        return ObjectGenType::NotFound;
    }

    inline ObjectSubType subtype() const noexcept {
        // return id & SUB_TYPE_MASK;
        // static constexpr uint64_t SUB_TYPE_MASK = 0xFC'00000000000000UL; // 0b1111'11'00
        // TODO:
        return ObjectSubType::NotFound;
    }

    inline uint64_t get_mod() const noexcept {
        return id & MOD_MASK;
    }

    inline uint64_t get_value() const noexcept {
        return id & VALUE_MASK;
    }

    inline bool is_null() const noexcept {
        return id == NULL_ID;
    }

    inline bool is_tmp() const noexcept {
        return (id & MOD_MASK) == MOD_TMP;
    }

    inline bool is_true() const noexcept {
        return id == BOOL_TRUE;
    }

    inline bool is_false() const noexcept {
        return id == BOOL_FALSE;
    }

    inline bool is_not_found() const noexcept {
        return id == MASK_NOT_FOUND;
    }

    inline bool is_valid() const noexcept {
        // Any other future "invalid" types should be added here.
        // Invalid types are those that should be skipped in contexts
        // such as aggregation functions.
        auto generic_type = this->generic_type();
        return (generic_type != ObjectGenType::Null && generic_type != ObjectGenType::NotFound);
    }

    inline bool operator==(const ObjectId& rhs) const noexcept {
        return id == rhs.id;
    }

    inline bool operator!=(const ObjectId& rhs) const noexcept {
        return id != rhs.id;
    }

    inline bool operator<(const ObjectId& rhs) const noexcept {
        return id < rhs.id;
    }

    inline bool operator>(const ObjectId& rhs) const noexcept {
        return id > rhs.id;
    }

    inline bool operator<=(const ObjectId& rhs) const noexcept {
        return id <= rhs.id;
    }

    inline bool operator>=(const ObjectId& rhs) const noexcept {
        return id >= rhs.id;
    }
};

std::ostream& operator<<(std::ostream& os, ObjectId oid);


static_assert(std::is_trivially_copyable<ObjectId>::value);
