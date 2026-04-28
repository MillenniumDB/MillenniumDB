#pragma once

#include <cstdint>
#include <ostream>

// clang-format off


enum class ObjectType : uint8_t {
    Null              = 0x00, // 0b000000'00

    // AnonInl also represents blank inlined in RDF, or node in GQL
    AnonInl           = 0x10, // 0b000100'00
    AnonTmp           = 0x12, // 0b000100'10

    NamedNodeInl      = 0x20, // 0b001000'00
    NamedNodeExt      = 0x21, // 0b001000'01
    NamedNodeTmp      = 0x22, // 0b001000'10
    NamedNodeHexExt   = 0x25, // 0b001001'01
    NamedNodeHexTmp   = 0x26, // 0b001001'10
    NamedNodeUuidExt  = 0x29, // 0b001010'01
    NamedNodeUuidTmp  = 0x2A, // 0b001010'10

    IriInl            = 0x30, // 0b001100'00
    IriExt            = 0x31, // 0b001100'01
    IriTmp            = 0x32, // 0b001100'10

    StringInl         = 0x40, // 0b010000'00
    StringExt         = 0x41, // 0b010000'01
    StringTmp         = 0x42, // 0b010000'10
    StringXsdInl      = 0x44, // 0b010001'00
    StringXsdExt      = 0x45, // 0b010001'01
    StringXsdTmp      = 0x46, // 0b010001'10
    StringLangInl     = 0x48, // 0b010010'00
    StringLangExt     = 0x49, // 0b010010'01
    StringLangTmp     = 0x4A, // 0b010010'10
    StringDatatypeInl = 0x4C, // 0b010011'00
    StringDatatypeExt = 0x4D, // 0b010011'01
    StringDatatypeTmp = 0x4E, // 0b010011'10

    NegativeInt56     = 0x50, // 0b010100'00
    PositiveInt56     = 0x51, // 0b010100'01 TODO: has ext, does this cause trouble in import?
    DecimalInl        = 0x54, // 0b010101'00
    DecimalExt        = 0x55, // 0b010101'01
    DecimalTmp        = 0x56, // 0b010101'10

    Float             = 0x58, // 0b010110'00
    DoubleExt         = 0x5D, // 0b010111'01
    DoubleTmp         = 0x5E, // 0b010111'10

    Date              = 0x60, // 0b011000'00
    Time              = 0x64, // 0b011001'00
    Datetime          = 0x68, // 0b011010'00
    Datetimestamp     = 0x6C, // 0b011011'00

    Bool              = 0x70, // 0b011100'00

    DirectedEdge      = 0x80, // 0b100000'00
    UndirectedEdge    = 0x84, // 0b100001'00
    NodeLabel         = 0x88, // 0b100010'00
    EdgeLabel         = 0x8C, // 0b100011'00
    NodeKey           = 0x90, // 0b100100'00
    EdgeKey           = 0x94, // 0b100101'00
    Direction         = 0x98, // 0b100110'00
    Path              = 0x9C, // 0b100111'00

    IriUuidLowerExt   = 0xA1, // 0b101000'01
    IriUuidLowerTmp   = 0xA2, // 0b101000'10
    IriUuidUpperExt   = 0xA5, // 0b101001'01
    IriUuidUpperTmp   = 0xA6, // 0b101001'10
    IriHexLowerExt    = 0xA9, // 0b101010'01
    IriHexLowerTmp    = 0xAA, // 0b101010'10
    IriHexUpperExt    = 0xAD, // 0b101011'01
    IriHexUpperTmp    = 0xAE, // 0b101011'10

    TensorFloatExt    = 0xB1, // 0b101100'01
    TensorFloatTmp    = 0xB2, // 0b101100'10
    TensorDoubleExt   = 0xB5, // 0b101110'01
    TensorDoubleTmp   = 0xB6, // 0b101110'10

    ListExt           = 0xC1, // 0b110000'01
    ListTmp           = 0xC2, // 0b110000'10
    DictionaryExt     = 0xD1, // 0b110100'01
    DictionaryTmp     = 0xD2, // 0b110100'10

    NotFound          = 0xFF, // 0b111111'11
};

enum class ObjectSubType {
    Null,
    Anon,
    NamedNode,
    Iri,
    String,
    StringXsd,
    StringLang,
    StringDatatype,
    Int,
    Decimal,
    Float,
    Double,
    TemporalLiteral, // Time, Date, DateTime, DateTimestamp,
    Bool,
    Edge,
    Path,
    PGMetaData, // EdgeKey, EdgeLabel, NodeKey, NodeLabel, Direction
    TensorFloat,
    TensorDouble,
    List,
    Dictionary,
    NotFound
};

enum class ObjectGenType {
    Null,
    Anon,
    NamedNode,
    Iri,
    String,
    Numeric,
    TemporalLiteral, // Time, Date, DateTime, DateTimestamp,
    Bool,
    Edge,
    Path,
    PGMetaData, // EdgeKey, EdgeLabel, NodeKey, NodeLabel, Direction
    Tensor,
    List,
    Dict,
    NotFound
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

    static constexpr uint64_t VALUE_MASK = 0x00'FFFFFFFFFFFFFFUL;
    static constexpr uint64_t TYPE_MASK  = 0xFF'00000000000000UL;

    static constexpr uint64_t MOD_MASK = 0x03'00000000000000UL; // 0b000000'11
    static constexpr uint64_t MOD_INL  = 0x00'00000000000000UL; // 0b000000'00
    static constexpr uint64_t MOD_EXT  = 0x01'00000000000000UL; // 0b000000'01
    static constexpr uint64_t MOD_TMP  = 0x02'00000000000000UL; // 0b000000'10

    static constexpr uint64_t MASK_LITERAL_TAG = 0x00'FFF00000000000UL; // 12 bits for language/datatype of string
    static constexpr uint64_t MASK_EXTERNAL_ID = 0x00'000FFFFFFFFFFFUL; // 44 least significant bits for external/tmp id

    static constexpr uint64_t MASK_IRI_PREFIX  = 0x00'FF000000000000UL;
    static constexpr uint64_t MASK_IRI_CONTENT = 0x00'00FFFFFFFFFFFFUL;

    static constexpr int IRI_INLINE_BYTES        = 6; // Number of bytes of IRI, excluding prefix id, stored inline
    static constexpr int STR_INLINE_BYTES        = 7; // Number of bytes of string stored inline
    static constexpr int NAMED_NODE_INLINE_BYTES = 7; // Number of bytes of named node stored inline
    static constexpr int STR_DT_INLINE_BYTES     = 5; // Number of bytes of string, excluding datatype id, stored inline
    static constexpr int STR_LANG_INLINE_BYTES   = 5; // Number of bytes of string, excluding language id, stored inline

    static constexpr uint64_t MASK_NULL                 = uint64_t(ObjectType::Null) << 56;
    static constexpr uint64_t MASK_ANON_INL             = uint64_t(ObjectType::AnonInl) << 56;
    static constexpr uint64_t MASK_ANON_TMP             = uint64_t(ObjectType::AnonTmp) << 56;
    static constexpr uint64_t MASK_NAMED_NODE_INL       = uint64_t(ObjectType::NamedNodeInl) << 56;
    static constexpr uint64_t MASK_NAMED_NODE_EXT       = uint64_t(ObjectType::NamedNodeExt) << 56;
    static constexpr uint64_t MASK_NAMED_NODE_TMP       = uint64_t(ObjectType::NamedNodeTmp) << 56;
    static constexpr uint64_t MASK_NAMED_NODE_HEX_EXT   = uint64_t(ObjectType::NamedNodeHexExt) << 56;
    static constexpr uint64_t MASK_NAMED_NODE_HEX_TMP   = uint64_t(ObjectType::NamedNodeHexTmp) << 56;
    static constexpr uint64_t MASK_NAMED_NODE_UUID_EXT  = uint64_t(ObjectType::NamedNodeUuidExt) << 56;
    static constexpr uint64_t MASK_NAMED_NODE_UUID_TMP  = uint64_t(ObjectType::NamedNodeUuidTmp) << 56;
    static constexpr uint64_t MASK_IRI_INL              = uint64_t(ObjectType::IriInl) << 56;
    static constexpr uint64_t MASK_IRI_EXT              = uint64_t(ObjectType::IriExt) << 56;
    static constexpr uint64_t MASK_IRI_TMP              = uint64_t(ObjectType::IriTmp) << 56;
    static constexpr uint64_t MASK_STR_INL              = uint64_t(ObjectType::StringInl) << 56;
    static constexpr uint64_t MASK_STR_EXT              = uint64_t(ObjectType::StringExt) << 56;
    static constexpr uint64_t MASK_STR_TMP              = uint64_t(ObjectType::StringTmp) << 56;
    static constexpr uint64_t MASK_STR_XSD_INL          = uint64_t(ObjectType::StringXsdInl) << 56;
    static constexpr uint64_t MASK_STR_XSD_EXT          = uint64_t(ObjectType::StringXsdExt) << 56;
    static constexpr uint64_t MASK_STR_XSD_TMP          = uint64_t(ObjectType::StringXsdTmp) << 56;
    static constexpr uint64_t MASK_STR_LANG_INL         = uint64_t(ObjectType::StringLangInl) << 56;
    static constexpr uint64_t MASK_STR_LANG_EXT         = uint64_t(ObjectType::StringLangExt) << 56;
    static constexpr uint64_t MASK_STR_LANG_TMP         = uint64_t(ObjectType::StringLangTmp) << 56;
    static constexpr uint64_t MASK_STR_DATATYPE_INL     = uint64_t(ObjectType::StringDatatypeInl) << 56;
    static constexpr uint64_t MASK_STR_DATATYPE_EXT     = uint64_t(ObjectType::StringDatatypeExt) << 56;
    static constexpr uint64_t MASK_STR_DATATYPE_TMP     = uint64_t(ObjectType::StringDatatypeTmp) << 56;
    static constexpr uint64_t MASK_NEGATIVE_INT         = uint64_t(ObjectType::NegativeInt56) << 56;
    static constexpr uint64_t MASK_POSITIVE_INT         = uint64_t(ObjectType::PositiveInt56) << 56;
    static constexpr uint64_t MASK_DECIMAL_INL          = uint64_t(ObjectType::DecimalInl) << 56;
    static constexpr uint64_t MASK_DECIMAL_EXT          = uint64_t(ObjectType::DecimalExt) << 56;
    static constexpr uint64_t MASK_DECIMAL_TMP          = uint64_t(ObjectType::DecimalTmp) << 56;
    static constexpr uint64_t MASK_FLOAT                = uint64_t(ObjectType::Float) << 56;
    static constexpr uint64_t MASK_DOUBLE_EXT           = uint64_t(ObjectType::DoubleExt) << 56;
    static constexpr uint64_t MASK_DOUBLE_TMP           = uint64_t(ObjectType::DoubleTmp) << 56;
    static constexpr uint64_t MASK_DT_DATE              = uint64_t(ObjectType::Date) << 56;
    static constexpr uint64_t MASK_DT_TIME              = uint64_t(ObjectType::Time) << 56;
    static constexpr uint64_t MASK_DT_DATETIME          = uint64_t(ObjectType::Datetime) << 56;
    static constexpr uint64_t MASK_DT_DATETIMESTAMP     = uint64_t(ObjectType::Datetimestamp) << 56;
    static constexpr uint64_t MASK_BOOL                 = uint64_t(ObjectType::Bool) << 56;
    static constexpr uint64_t MASK_DIRECTED_EDGE        = uint64_t(ObjectType::DirectedEdge) << 56;
    static constexpr uint64_t MASK_UNDIRECTED_EDGE      = uint64_t(ObjectType::UndirectedEdge) << 56;
    static constexpr uint64_t MASK_NODE_LABEL           = uint64_t(ObjectType::NodeLabel) << 56;
    static constexpr uint64_t MASK_EDGE_LABEL           = uint64_t(ObjectType::EdgeLabel) << 56;
    static constexpr uint64_t MASK_NODE_KEY             = uint64_t(ObjectType::NodeKey) << 56;
    static constexpr uint64_t MASK_EDGE_KEY             = uint64_t(ObjectType::EdgeKey) << 56;
    static constexpr uint64_t MASK_DIRECTION            = uint64_t(ObjectType::Direction) << 56;
    static constexpr uint64_t MASK_PATH                 = uint64_t(ObjectType::Path) << 56;
    static constexpr uint64_t MASK_IRI_UUID_LOWER_EXT   = uint64_t(ObjectType::IriUuidLowerExt) << 56;
    static constexpr uint64_t MASK_IRI_UUID_LOWER_TMP   = uint64_t(ObjectType::IriUuidLowerTmp) << 56;
    static constexpr uint64_t MASK_IRI_UUID_UPPER_EXT   = uint64_t(ObjectType::IriUuidUpperExt) << 56;
    static constexpr uint64_t MASK_IRI_UUID_UPPER_TMP   = uint64_t(ObjectType::IriUuidUpperTmp) << 56;
    static constexpr uint64_t MASK_IRI_HEX_LOWER_EXT    = uint64_t(ObjectType::IriHexLowerExt) << 56;
    static constexpr uint64_t MASK_IRI_HEX_LOWER_TMP    = uint64_t(ObjectType::IriHexLowerTmp) << 56;
    static constexpr uint64_t MASK_IRI_HEX_UPPER_EXT    = uint64_t(ObjectType::IriHexUpperExt) << 56;
    static constexpr uint64_t MASK_IRI_HEX_UPPER_TMP    = uint64_t(ObjectType::IriHexUpperTmp) << 56;
    static constexpr uint64_t MASK_TENSOR_FLOAT_EXT     = uint64_t(ObjectType::TensorFloatExt) << 56;
    static constexpr uint64_t MASK_TENSOR_FLOAT_TMP     = uint64_t(ObjectType::TensorFloatTmp) << 56;
    static constexpr uint64_t MASK_TENSOR_DOUBLE_EXT    = uint64_t(ObjectType::TensorDoubleExt) << 56;
    static constexpr uint64_t MASK_TENSOR_DOUBLE_TMP    = uint64_t(ObjectType::TensorDoubleTmp) << 56;
    static constexpr uint64_t MASK_LIST_EXT             = uint64_t(ObjectType::ListExt) << 56;
    static constexpr uint64_t MASK_LIST_TMP             = uint64_t(ObjectType::ListTmp) << 56;
    static constexpr uint64_t MASK_DICTIONARY_EXT       = uint64_t(ObjectType::DictionaryExt) << 56;
    static constexpr uint64_t MASK_DICTIONARY_TMP       = uint64_t(ObjectType::DictionaryTmp) << 56;
    static constexpr uint64_t MASK_NOT_FOUND            = uint64_t(ObjectType::NotFound) << 56;

    static constexpr uint64_t DIRECTION_RIGHT      = MASK_DIRECTION | 0x0UL;
    static constexpr uint64_t DIRECTION_LEFT       = MASK_DIRECTION | 0x1UL;
    static constexpr uint64_t DIRECTION_UNDIRECTED = MASK_DIRECTION | 0x2UL;

    static_assert(MASK_NEGATIVE_INT < MASK_POSITIVE_INT, "Integers won't be ordered properly in the B+Tree.");
    static_assert(MASK_NEGATIVE_INT < 0x80'00000000000000UL, "Integer IDs can't be subtracted without overflow.");
    static_assert(MASK_POSITIVE_INT < 0x80'00000000000000UL, "Integer IDs can't be subtracted without overflow.");

    static constexpr uint64_t NULL_ID = MASK_NULL;
    // clang-format on

    uint64_t id;

    explicit constexpr ObjectId(uint64_t id) :
        id(id)
    { }

    ObjectId() = default;

    static constexpr ObjectId get_null() noexcept
    {
        return ObjectId(NULL_ID);
    }

    static constexpr ObjectId get_not_found() noexcept
    {
        return ObjectId(MASK_NOT_FOUND);
    }

    static constexpr ObjectId get_false() noexcept
    {
        return ObjectId(MASK_BOOL | 0UL);
    }
    static constexpr ObjectId get_true() noexcept
    {
        return ObjectId(MASK_BOOL | 1UL);
    }

    inline ObjectType type() const noexcept
    {
        return static_cast<ObjectType>(id >> 56);
    }

    inline ObjectGenType generic_type() const noexcept
    {
        switch (type()) {
        case ObjectType::Null:
            return ObjectGenType::Null;
        case ObjectType::AnonInl:
        case ObjectType::AnonTmp:
            return ObjectGenType::Anon;
        case ObjectType::NamedNodeInl:
        case ObjectType::NamedNodeExt:
        case ObjectType::NamedNodeTmp:
        case ObjectType::NamedNodeHexExt:
        case ObjectType::NamedNodeHexTmp:
        case ObjectType::NamedNodeUuidExt:
        case ObjectType::NamedNodeUuidTmp:
            return ObjectGenType::NamedNode;
        case ObjectType::IriInl:
        case ObjectType::IriExt:
        case ObjectType::IriTmp:
            return ObjectGenType::Iri;
        case ObjectType::StringInl:
        case ObjectType::StringExt:
        case ObjectType::StringTmp:
        case ObjectType::StringXsdInl:
        case ObjectType::StringXsdExt:
        case ObjectType::StringXsdTmp:
        case ObjectType::StringLangInl:
        case ObjectType::StringLangExt:
        case ObjectType::StringLangTmp:
        case ObjectType::StringDatatypeInl:
        case ObjectType::StringDatatypeExt:
        case ObjectType::StringDatatypeTmp:
            return ObjectGenType::String;
        case ObjectType::NegativeInt56:
        case ObjectType::PositiveInt56:
        case ObjectType::DecimalInl:
        case ObjectType::DecimalExt:
        case ObjectType::DecimalTmp:
        case ObjectType::Float:
        case ObjectType::DoubleExt:
        case ObjectType::DoubleTmp:
            return ObjectGenType::Numeric;
        case ObjectType::Date:
        case ObjectType::Time:
        case ObjectType::Datetime:
        case ObjectType::Datetimestamp:
            return ObjectGenType::TemporalLiteral;
        case ObjectType::Bool:
            return ObjectGenType::Bool;
        case ObjectType::DirectedEdge:
        case ObjectType::UndirectedEdge:
            return ObjectGenType::Edge;
        case ObjectType::NodeLabel:
        case ObjectType::EdgeLabel:
        case ObjectType::NodeKey:
        case ObjectType::EdgeKey:
        case ObjectType::Direction:
            return ObjectGenType::PGMetaData;
        case ObjectType::Path:
            return ObjectGenType::Path;
        case ObjectType::IriUuidLowerExt:
        case ObjectType::IriUuidLowerTmp:
        case ObjectType::IriUuidUpperExt:
        case ObjectType::IriUuidUpperTmp:
        case ObjectType::IriHexLowerExt:
        case ObjectType::IriHexLowerTmp:
        case ObjectType::IriHexUpperExt:
        case ObjectType::IriHexUpperTmp:
            return ObjectGenType::Iri;
        case ObjectType::TensorFloatExt:
        case ObjectType::TensorFloatTmp:
        case ObjectType::TensorDoubleExt:
        case ObjectType::TensorDoubleTmp:
            return ObjectGenType::Tensor;
        case ObjectType::ListExt:
        case ObjectType::ListTmp:
            return ObjectGenType::List;
        case ObjectType::DictionaryExt:
        case ObjectType::DictionaryTmp:
            return ObjectGenType::Dict;
        case ObjectType::NotFound:
            return ObjectGenType::NotFound;
        }
        return ObjectGenType::NotFound;
    }

    inline ObjectSubType subtype() const noexcept
    {
        switch (type()) {
        case ObjectType::Null:
            return ObjectSubType::Null;
        case ObjectType::AnonInl:
        case ObjectType::AnonTmp:
            return ObjectSubType::Anon;
        case ObjectType::NamedNodeInl:
        case ObjectType::NamedNodeExt:
        case ObjectType::NamedNodeTmp:
        case ObjectType::NamedNodeHexExt:
        case ObjectType::NamedNodeHexTmp:
        case ObjectType::NamedNodeUuidExt:
        case ObjectType::NamedNodeUuidTmp:
            return ObjectSubType::NamedNode;
        case ObjectType::IriInl:
        case ObjectType::IriExt:
        case ObjectType::IriTmp:
            return ObjectSubType::Iri;
        case ObjectType::StringInl:
        case ObjectType::StringExt:
        case ObjectType::StringTmp:
            return ObjectSubType::String;
        case ObjectType::StringXsdInl:
        case ObjectType::StringXsdExt:
        case ObjectType::StringXsdTmp:
            return ObjectSubType::StringXsd;
        case ObjectType::StringLangInl:
        case ObjectType::StringLangExt:
        case ObjectType::StringLangTmp:
            return ObjectSubType::StringLang;
        case ObjectType::StringDatatypeInl:
        case ObjectType::StringDatatypeExt:
        case ObjectType::StringDatatypeTmp:
            return ObjectSubType::StringDatatype;
        case ObjectType::NegativeInt56:
        case ObjectType::PositiveInt56:
            return ObjectSubType::Int;
        case ObjectType::DecimalInl:
        case ObjectType::DecimalExt:
        case ObjectType::DecimalTmp:
            return ObjectSubType::Decimal;
        case ObjectType::Float:
            return ObjectSubType::Float;
        case ObjectType::DoubleExt:
        case ObjectType::DoubleTmp:
            return ObjectSubType::Double;
        case ObjectType::Date:
        case ObjectType::Time:
        case ObjectType::Datetime:
        case ObjectType::Datetimestamp:
            return ObjectSubType::TemporalLiteral;
        case ObjectType::Bool:
            return ObjectSubType::Bool;
        case ObjectType::DirectedEdge:
        case ObjectType::UndirectedEdge:
            return ObjectSubType::Edge;
        case ObjectType::NodeLabel:
        case ObjectType::EdgeLabel:
        case ObjectType::NodeKey:
        case ObjectType::EdgeKey:
        case ObjectType::Direction:
            return ObjectSubType::PGMetaData;
        case ObjectType::Path:
            return ObjectSubType::Path;
        case ObjectType::IriUuidLowerExt:
        case ObjectType::IriUuidLowerTmp:
        case ObjectType::IriUuidUpperExt:
        case ObjectType::IriUuidUpperTmp:
        case ObjectType::IriHexLowerExt:
        case ObjectType::IriHexLowerTmp:
        case ObjectType::IriHexUpperExt:
        case ObjectType::IriHexUpperTmp:
            return ObjectSubType::Iri;
        case ObjectType::TensorFloatExt:
        case ObjectType::TensorFloatTmp:
            return ObjectSubType::TensorFloat;
        case ObjectType::TensorDoubleExt:
        case ObjectType::TensorDoubleTmp:
            return ObjectSubType::TensorDouble;
        case ObjectType::ListExt:
        case ObjectType::ListTmp:
            return ObjectSubType::List;
        case ObjectType::DictionaryExt:
        case ObjectType::DictionaryTmp:
            return ObjectSubType::Dictionary;
        case ObjectType::NotFound:
            return ObjectSubType::NotFound;
        }
        return ObjectSubType::NotFound;
    }

    inline uint64_t get_mod() const noexcept
    {
        return id & MOD_MASK;
    }

    inline uint64_t get_value() const noexcept
    {
        return id & VALUE_MASK;
    }

    inline bool is_null() const noexcept
    {
        return id == NULL_ID;
    }

    inline bool is_tmp() const noexcept
    {
        return (id & MOD_MASK) == MOD_TMP;
    }

    inline bool is_true() const noexcept
    {
        return *this == get_true();
    }

    inline bool is_false() const noexcept
    {
        return *this == get_false();
    }

    inline bool is_not_found() const noexcept
    {
        return id == MASK_NOT_FOUND;
    }

    inline bool is_valid() const noexcept
    {
        // Any other future "invalid" types should be added here.
        // Invalid types are those that should be skipped in contexts
        // such as aggregation functions.
        auto generic_type = this->generic_type();
        return (generic_type != ObjectGenType::Null && generic_type != ObjectGenType::NotFound);
    }

    inline bool constexpr operator==(const ObjectId& rhs) const noexcept
    {
        return id == rhs.id;
    }

    inline bool constexpr operator!=(const ObjectId& rhs) const noexcept
    {
        return id != rhs.id;
    }

    inline bool constexpr operator<(const ObjectId& rhs) const noexcept
    {
        return id < rhs.id;
    }

    inline bool constexpr operator>(const ObjectId& rhs) const noexcept
    {
        return id > rhs.id;
    }

    inline bool constexpr operator<=(const ObjectId& rhs) const noexcept
    {
        return id <= rhs.id;
    }

    inline bool constexpr operator>=(const ObjectId& rhs) const noexcept
    {
        return id >= rhs.id;
    }
};

std::ostream& operator<<(std::ostream& os, ObjectId oid);

static_assert(std::is_trivially_copyable<ObjectId>::value);
