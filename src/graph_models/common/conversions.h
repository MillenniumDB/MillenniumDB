#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <sstream>

#include "graph_models/object_id.h"
#include "graph_models/common/datatypes/datetime.h"
#include "query/exceptions.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

namespace Common { namespace Conversions {

constexpr int64_t INTEGER_MAX = 0x00FF'FFFF'FFFF'FFFFL;

constexpr ObjectId pack_bool(bool b) {
    return b ? ObjectId(ObjectId::MASK_BOOL | 1)
             : ObjectId(ObjectId::MASK_BOOL | 0);
}

constexpr bool unpack_bool(ObjectId oid) {
    return oid.id != ObjectId::BOOL_FALSE;
}

constexpr ObjectId pack_int(int64_t i) {
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

inline int64_t unpack_int(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_NEGATIVE_INT:
        return static_cast<int64_t>((~oid.id) & ObjectId::VALUE_MASK) * -1;
    case ObjectId::MASK_POSITIVE_INT:
        return static_cast<int64_t>(oid.get_value());
    default:
        throw LogicException("Called unpack_int with incorrect ObjectId type, this should never happen");
    }
}

inline ObjectId pack_float(float f) {
    auto src = reinterpret_cast<unsigned char*>(&f);

    auto oid = ObjectId::MASK_FLOAT;
    oid |= static_cast<uint64_t>(src[0]);
    oid |= static_cast<uint64_t>(src[1]) << 8;
    oid |= static_cast<uint64_t>(src[2]) << 16;
    oid |= static_cast<uint64_t>(src[3]) << 24;

    return ObjectId(oid);
}

inline float unpack_float(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_FLOAT);

    auto  value = oid.id;
    float flt;
    auto  dst = reinterpret_cast<char*>(&flt);

    dst[0] = value & 0xFF;
    dst[1] = (value >> 8) & 0xFF;
    dst[2] = (value >> 16) & 0xFF;
    dst[3] = (value >> 24) & 0xFF;

    return flt;
}


inline double unpack_double(ObjectId oid) {
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

constexpr ObjectId pack_date(const DateTime& dt) {
    return ObjectId(dt.id);
}

constexpr DateTime unpack_date(ObjectId oid) {
    return DateTime(oid);
}

constexpr uint64_t get_path_id(ObjectId oid) {
    return oid.id & 0x00FF'FFFF'FFFF'FFFFUL;
}

}} // namespace Common::Conversions
