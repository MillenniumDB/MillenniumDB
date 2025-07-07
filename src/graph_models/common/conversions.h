#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/common/datatypes/decimal.h"
#include "graph_models/common/datatypes/tensor/tensor.h"
#include "graph_models/object_id.h"
#include "query/exceptions.h"
#include "system/string_manager.h"
#include "system/tensor_manager.h"
#include "system/tmp_manager.h"

namespace Common { namespace Conversions {

constexpr int64_t INTEGER_MAX = 0x00FF'FFFF'FFFF'FFFFL;

constexpr ObjectId pack_bool(bool b)
{
    return b ? ObjectId(ObjectId::MASK_BOOL | 1) : ObjectId(ObjectId::MASK_BOOL | 0);
}

constexpr bool unpack_bool(ObjectId oid)
{
    return oid.id != ObjectId::BOOL_FALSE;
}

constexpr ObjectId pack_int(int64_t i)
{
    uint64_t oid = ObjectId::MASK_POSITIVE_INT;

    if (i < 0) {
        oid = ObjectId::MASK_NEGATIVE_INT;
        i *= -1;
        if (i > INTEGER_MAX) {
            return ObjectId::get_null();
        }
        i = (~i) & ObjectId::VALUE_MASK;
    } else {
        if (i > INTEGER_MAX) {
            return ObjectId::get_null();
        }
    }

    return ObjectId(oid | i);
}

inline int64_t unpack_int(ObjectId oid)
{
    switch (oid.get_type()) {
    case ObjectId::MASK_NEGATIVE_INT:
        return static_cast<int64_t>((~oid.id) & ObjectId::VALUE_MASK) * -1;
    case ObjectId::MASK_POSITIVE_INT:
        return static_cast<int64_t>(oid.get_value());
    default:
        throw LogicException("Called unpack_int with incorrect ObjectId type, this should never happen");
    }
}

inline ObjectId pack_float(float f)
{
    auto src = reinterpret_cast<unsigned char*>(&f);

    auto oid = ObjectId::MASK_FLOAT;
    oid |= static_cast<uint64_t>(src[0]);
    oid |= static_cast<uint64_t>(src[1]) << 8;
    oid |= static_cast<uint64_t>(src[2]) << 16;
    oid |= static_cast<uint64_t>(src[3]) << 24;

    return ObjectId(oid);
}

inline ObjectId pack_decimal(Decimal dec)
{
    if (dec.can_inline()) {
        return ObjectId(ObjectId::MASK_DECIMAL_INLINED | dec.serialize_inlined());
    }

    char dec_buffer[Decimal::EXTERN_BUFFER_SIZE];
    dec.serialize_extern(dec_buffer);
    auto str_id = string_manager.get_bytes_id(dec_buffer, Decimal::EXTERN_BUFFER_SIZE);
    uint64_t oid;
    if (str_id != ObjectId::MASK_NOT_FOUND) {
        oid = ObjectId::MASK_DECIMAL_EXTERN | str_id;
    } else {
        oid = ObjectId::MASK_DECIMAL_TMP | tmp_manager.get_bytes_id(dec_buffer, Decimal::EXTERN_BUFFER_SIZE);
    }
    return ObjectId(oid);
}

inline ObjectId pack_double(double dbl)
{
    uint64_t oid;
    auto bytes = reinterpret_cast<const char*>(reinterpret_cast<const char*>(&dbl));
    auto bytes_id = string_manager.get_bytes_id(bytes, sizeof(double));
    if (bytes_id != ObjectId::MASK_NOT_FOUND) {
        oid = ObjectId::MASK_DOUBLE_EXTERN | bytes_id;
    } else {
        oid = ObjectId::MASK_DOUBLE_TMP | tmp_manager.get_bytes_id(bytes, sizeof(double));
    }
    return ObjectId(oid);
}

inline float unpack_float(ObjectId oid)
{
    assert(oid.get_type() == ObjectId::MASK_FLOAT);

    auto value = oid.id;
    float flt;
    auto dst = reinterpret_cast<char*>(&flt);

    dst[0] = value & 0xFF;
    dst[1] = (value >> 8) & 0xFF;
    dst[2] = (value >> 16) & 0xFF;
    dst[3] = (value >> 24) & 0xFF;

    return flt;
}

inline double unpack_double(ObjectId oid)
{
    assert(oid.get_sub_type() == ObjectId::MASK_DOUBLE);

    std::stringstream ss;
    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;

    switch (oid.get_mod()) {
    case ObjectId::MOD_TMP: {
        tmp_manager.print_str(ss, external_id);
        break;
    }
    case ObjectId::MOD_EXTERNAL: {
        string_manager.print(ss, external_id);
        break;
    }
    }

    double dbl;
    auto dst = reinterpret_cast<char*>(&dbl);
    ss.read(dst, 8);

    return dbl;
}

/*
Steps to evaluate an expression:
    - Calculate the datatype the operation should use (calculate_optype)
    - unpack operands (unpack_x)
    - convert operands to previously calculated datatype
    - evaluate operation
    - pack result (pack_x)

Type promotion and type substitution order:
    integer -> decimal -> float (-> double)

Conversion:
    int64_t -> Decimal (Decimal constructor)
    int64_t -> float (cast)
    Decimal -> float (Decimal member function)
*/

inline Decimal unpack_decimal_inlined(ObjectId oid)
{
    assert(oid.get_type() == ObjectId::MASK_DECIMAL_INLINED);
    return Decimal::from_inlined(oid.get_value());
}

inline Decimal unpack_decimal(ObjectId oid)
{
    switch (oid.get_type()) {
    case ObjectId::MASK_DECIMAL_INLINED:
        return unpack_decimal_inlined(oid);
    case ObjectId::MASK_DECIMAL_EXTERN: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        char buffer[Decimal::EXTERN_BUFFER_SIZE];
        string_manager.print_to_buffer(buffer, external_id);
        return Decimal::from_external(buffer);
    }

    case ObjectId::MASK_DECIMAL_TMP: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        char buffer[Decimal::EXTERN_BUFFER_SIZE];
        tmp_manager.print_to_buffer(buffer, external_id);
        return Decimal::from_external(buffer);
    }
    default:
        throw LogicException("Called unpack_decimal with incorrect ObjectId type, this should never happen");
    }
}

constexpr ObjectId pack_date(const DateTime& dt)
{
    return ObjectId(dt.id);
}

constexpr DateTime unpack_date(ObjectId oid)
{
    return DateTime(oid);
}

constexpr uint64_t get_path_id(ObjectId oid)
{
    return oid.id & 0x00FF'FFFF'FFFF'FFFFUL;
}

// The order, int < dec < flt < ...tensors < inv is important
enum class OpType {
    INTEGER = 0x01,
    DECIMAL = 0x02,
    FLOAT = 0x03,
    DOUBLE = 0x04,
    TENSOR_FLOAT = 0x05,
    TENSOR_DOUBLE = 0x06,
    INVALID = 0x07,
};

/**
 *  @brief Calculates the generic datatypes of the operand in an expression.
 *  @param oid ObjectId of the operand involved in an expression.
 *  @return generic numeric datatype of the operand or OPTYPE_INVALID if oid is not numeric
 */
inline OpType calculate_optype(ObjectId oid)
{
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return OpType::INTEGER;
    case ObjectId::MASK_DECIMAL:
        return OpType::DECIMAL;
    case ObjectId::MASK_FLOAT:
        return OpType::FLOAT;
    case ObjectId::MASK_DOUBLE:
        return OpType::DOUBLE;
    case ObjectId::MASK_TENSOR_FLOAT:
        return OpType::TENSOR_FLOAT;
    case ObjectId::MASK_TENSOR_DOUBLE:
        return OpType::TENSOR_FLOAT;
    default:
        return OpType::INVALID;
    }
}

/**
 *  @brief Calculates the datatype that should be used for expression evaluation.
 *  @param oid1 ObjectId of the first operand.
 *  @param oid2 ObjectId of the second operand.
 *  @return datatype that should be used or OPTYPE_INVALID if not both operands are numeric.
 */
inline OpType calculate_optype(ObjectId oid1, ObjectId oid2)
{
    auto optype1 = calculate_optype(oid1);
    auto optype2 = calculate_optype(oid2);
    return std::max(optype1, optype2);
}

template<typename T>
inline ObjectId pack_tensor(const tensor::Tensor<T>& tensor)
{
    if (tensor.empty()) {
        return ObjectId(tensor::Tensor<T>::get_inline_mask());
    }

    const auto bytes = reinterpret_cast<const char*>(tensor.data());
    const auto num_bytes = sizeof(T) * tensor.size();

    const uint64_t tensor_id = tensor_manager.get_id(bytes, num_bytes);
    if (tensor_id != ObjectId::MASK_NOT_FOUND) {
        return ObjectId(tensor::Tensor<T>::get_external_mask() | tensor_id);
    }

    return ObjectId(tensor::Tensor<T>::get_tmp_mask() | tmp_manager.get_bytes_id(bytes, num_bytes));
}

template<typename T>
inline tensor::Tensor<T> unpack_tensor(ObjectId oid)
{
    // No need to handle tensor construction errors in runtime as they would be an implementation fault and they
    // should never happen
    switch (oid.get_type()) {
    case tensor::Tensor<T>::get_inline_mask(): {
        return tensor::Tensor<T>();
    }
    case tensor::Tensor<T>::get_external_mask(): {
        return tensor_manager.get_tensor<T>(oid);
    }
    case tensor::Tensor<T>::get_tmp_mask(): {
        const uint64_t tmp_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        const std::string& tensor_bytes = tmp_manager.get_str(tmp_id);
        return tensor::Tensor<T>::from_bytes(tensor_bytes.data(), tensor_bytes.size());
    }
    default:
        throw LogicException("Called unpack_tensor with incorrect ObjectId type, this should never happen");
    }
}

template<typename T>
inline tensor::Tensor<T> to_tensor(ObjectId oid)
{
    switch (oid.get_type()) {
    case tensor::Tensor<float>::get_inline_mask():
    case tensor::Tensor<float>::get_external_mask():
    case tensor::Tensor<float>::get_tmp_mask(): {
        const auto tensor = Common::Conversions::unpack_tensor<float>(oid);
        if constexpr (std::is_same_v<T, float>) {
            // Prevent unnecessary cast
            return tensor;
        }
        return tensor.cast<T>();
    }
    case tensor::Tensor<double>::get_inline_mask():
    case tensor::Tensor<double>::get_external_mask():
    case tensor::Tensor<double>::get_tmp_mask(): {
        const auto tensor = Common::Conversions::unpack_tensor<double>(oid);
        if constexpr (std::is_same_v<T, double>) {
            // Prevent unnecessary cast
            return tensor;
        }
        return tensor.cast<T>();
    }
    default:
        throw LogicException("Called to_tensor with incorrect ObjectId type, this should never happen");
    }
}

/**
 *  @brief Converts an ObjectId to int64_t if permitted.
 *  @param oid ObjectId to convert.
 *  @return an int64_t representing the ObjectId.
 *  @throws LogicException if the ObjectId has no permitted type.
 */
inline int64_t to_integer(ObjectId oid)
{
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return unpack_int(oid);
    default:
        throw LogicException("Called to_integer with incorrect ObjectId type, this should never happen");
    }
}

/**
 *  @brief Converts an ObjectId to Decimal if permitted.
 *  @param oid ObjectId to convert.
 *  @return a Decimal representing the ObjectId.
 *  @throws LogicException if the ObjectId has no permitted type.
 */
inline Decimal to_decimal(ObjectId oid)
{
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return Decimal(unpack_int(oid));
    case ObjectId::MASK_DECIMAL:
        return unpack_decimal(oid);
    default:
        throw LogicException("Called to_decimal with incorrect ObjectId type, this should never happen");
    }
}

/**
 *  @brief Converts an ObjectId to float if permitted.
 *  @param oid ObjectId to convert.
 *  @return a float representing the ObjectId.
 *  @throws LogicException if the ObjectId has no permitted type.
 */
inline float to_float(ObjectId oid)
{
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return unpack_int(oid);
    case ObjectId::MASK_DECIMAL:
        return unpack_decimal(oid).to_float();
    case ObjectId::MASK_FLOAT:
        return unpack_float(oid);
    case ObjectId::MASK_DOUBLE:
        return unpack_double(oid);
    default:
        throw LogicException("Called to_float with incorrect ObjectId type, this should never happen");
    }
}

/**
 *  @brief Converts an ObjectId to double if permitted.
 *  @param oid ObjectId to convert.
 *  @return a double representing the ObjectId.
 *  @throws LogicException if the ObjectId has no permitted type.
 */
inline double to_double(ObjectId oid)
{
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return unpack_int(oid);
    case ObjectId::MASK_DECIMAL:
        return unpack_decimal(oid).to_double();
    case ObjectId::MASK_FLOAT:
        return unpack_float(oid);
    case ObjectId::MASK_DOUBLE:
        return unpack_double(oid);
    default:
        throw LogicException("Called to_double with incorrect ObjectId type, this should never happen");
    }
}

}} // namespace Common::Conversions
