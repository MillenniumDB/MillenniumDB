#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/common/datatypes/tensor.h"
#include "graph_models/object_id.h"
#include "query/exceptions.h"
#include "query/query_context.h"
#include "system/string_manager.h"
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

template<typename T>
inline ObjectId pack_tensor(const Tensor<T>& tensor)
{
    if (tensor.empty()) {
        return ObjectId(Tensor<T>::get_inline_mask());
    }

    const auto bytes = reinterpret_cast<const char*>(tensor.data());
    const auto bytes_size = sizeof(T) * tensor.size();
    const auto str_id = string_manager.get_bytes_id(bytes, bytes_size);
    if (str_id != ObjectId::MASK_NOT_FOUND) {
        return ObjectId(Tensor<T>::get_external_mask() | str_id);
    }

    return ObjectId(Tensor<T>::get_tmp_mask() | tmp_manager.get_bytes_id(bytes, bytes_size));
}

template<typename T>
inline Tensor<T> unpack_tensor(ObjectId oid)
{
    // No need to handle tensor construction errors in runtime as they would be an implementation fault and they
    // should never happen
    switch (oid.get_type()) {
    case Tensor<T>::get_inline_mask(): {
        return Tensor<T>();
    }
    case Tensor<T>::get_external_mask(): {
        const uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        auto buffer = get_query_ctx().get_buffer1();
        const auto size = string_manager.print_to_buffer(buffer, external_id);
        bool error;
        return Tensor<T>::from_external(buffer, size, &error);
    }
    case Tensor<T>::get_tmp_mask(): {
        const uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        auto buffer = get_query_ctx().get_buffer1();
        const auto size = tmp_manager.print_to_buffer(buffer, external_id);
        bool error;
        return Tensor<T>::from_external(buffer, size, &error);
    }
    default:
        throw LogicException("Called unpack_tensor with incorrect ObjectId type, this should never happen");
    }
}

template<typename T>
inline Tensor<T> to_tensor(ObjectId oid)
{
    switch (oid.get_type()) {
    case Tensor<float>::get_inline_mask():
    case Tensor<float>::get_external_mask():
    case Tensor<float>::get_tmp_mask(): {
        if constexpr (std::is_same_v<T, float>) {
            // Prevent unnecessary copy
            return Common::Conversions::unpack_tensor<float>(oid);
        }
        const auto tensor = Common::Conversions::unpack_tensor<float>(oid);
        std::vector<T, AlignedAllocator<T>> casted_data;
        casted_data.resize(tensor.size());
        for (std::size_t i = 0; i < tensor.size(); ++i) {
            casted_data[i] = static_cast<T>(tensor[i]);
        }
        return Tensor<T>(std::move(casted_data));
    }
    case Tensor<double>::get_inline_mask():
    case Tensor<double>::get_external_mask():
    case Tensor<double>::get_tmp_mask(): {
        if constexpr (std::is_same_v<T, double>) {
            // Prevent unnecessary copy
            return Common::Conversions::unpack_tensor<double>(oid);
        }
        const auto tensor = Common::Conversions::unpack_tensor<double>(oid);
        std::vector<T, AlignedAllocator<T>> casted_data;
        casted_data.resize(tensor.size());
        for (std::size_t i = 0; i < tensor.size(); ++i) {
            casted_data[i] = static_cast<T>(tensor[i]);
        }
        return Tensor<T>(std::move(casted_data));
    }
    default:
        throw LogicException("Called to_tensor with incorrect ObjectId type, this should never happen");
    }
}
}} // namespace Common::Conversions
