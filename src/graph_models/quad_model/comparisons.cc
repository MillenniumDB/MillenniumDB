#include "comparisons.h"

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/quad_model/conversions.h"
#include "system/string_manager.h"

using namespace MQL;

int64_t Comparisons::compare(ObjectId lhs, ObjectId rhs)
{
    const auto lhs_generic_type = lhs.generic_type();
    const auto rhs_generic_type = rhs.generic_type();

    if (lhs_generic_type != rhs_generic_type) {
        return static_cast<int64_t>(lhs_generic_type) - static_cast<int64_t>(rhs_generic_type);
    }

    const auto lhs_unmasked_id = lhs.id & ObjectId::VALUE_MASK;
    const auto rhs_unmasked_id = rhs.id & ObjectId::VALUE_MASK;

    switch (lhs_generic_type) {
    case ObjectGenType::Null: {
        return 0;
    }
    case ObjectGenType::String:
    case ObjectGenType::NamedNode: {
        auto lhs_buffer = get_query_ctx().get_buffer1();
        auto rhs_buffer = get_query_ctx().get_buffer2();

        auto lhs_size = Conversions::print_string(lhs, lhs_buffer);
        auto rhs_size = Conversions::print_string(rhs, rhs_buffer);

        return StringManager::compare(lhs_buffer, rhs_buffer, lhs_size, rhs_size);
    }
    case ObjectGenType::Numeric: {
        double lhs_value;
        double rhs_value;

        const auto lhs_type = lhs.type();
        const auto rhs_type = rhs.type();

        switch (lhs_type) {
        case ObjectType::NegativeInt56: {
            int64_t i = (~lhs.id) & 0x00FF'FFFF'FFFF'FFFFUL;
            lhs_value = i * -1;
            break;
        }
        case ObjectType::PositiveInt56: {
            int64_t i = lhs_unmasked_id;
            lhs_value = i;
            break;
        }
        case ObjectType::Float: {
            lhs_value = Conversions::unpack_float(lhs);
            break;
        }
        case ObjectType::DoubleExt:
        case ObjectType::DoubleTmp: {
            lhs_value = Conversions::unpack_double(lhs);
            break;
        }
        case ObjectType::DecimalInl:
        case ObjectType::DecimalExt:
        case ObjectType::DecimalTmp: {
            lhs_value = Common::Conversions::unpack_decimal(lhs).to_double();
            break;
        }
        default:
            throw std::logic_error("Unmanaged NUMERIC type " + to_string(lhs_type));
        }

        switch (rhs_type) {
        case ObjectType::NegativeInt56: {
            int64_t i = (~rhs.id) & 0x00FF'FFFF'FFFF'FFFFUL;
            rhs_value = i * -1;
            break;
        }
        case ObjectType::PositiveInt56: {
            int64_t i = rhs_unmasked_id;
            rhs_value = i;
            break;
        }
        case ObjectType::Float: {
            rhs_value = Conversions::unpack_float(rhs);
            break;
        }
        case ObjectType::DoubleExt:
        case ObjectType::DoubleTmp: {
            rhs_value = Conversions::unpack_double(rhs);
            break;
        }
        case ObjectType::DecimalInl:
        case ObjectType::DecimalExt:
        case ObjectType::DecimalTmp: {
            rhs_value = Common::Conversions::unpack_decimal(rhs).to_double();
            break;
        }
        default:
            throw std::logic_error("Unmanaged NUMERIC type " + to_string(lhs_type));
        }
        if (lhs_value == rhs_value) {
            return 0;
        }
        return lhs_value < rhs_value ? -1 : 1;
    }
    case ObjectGenType::TemporalLiteral: {
        DateTime lhs_dt(lhs);
        DateTime rhs_dt(rhs);
        return lhs_dt.MQL_compare(rhs_dt);
    }
    case ObjectGenType::Tensor: {
        const auto optype = Conversions::calculate_optype(lhs, rhs);
        switch (optype) {
        case Conversions::OpType::TENSOR_FLOAT: {
            const auto lhs_value = Conversions::to_tensor<float>(lhs);
            const auto rhs_value = Conversions::to_tensor<float>(rhs);
            return tensor::Tensor<float>::compare(lhs_value, rhs_value);
        }
        case Conversions::OpType::TENSOR_DOUBLE: {
            const auto lhs_value = Conversions::to_tensor<double>(lhs);
            const auto rhs_value = Conversions::to_tensor<double>(rhs);
            return tensor::Tensor<double>::compare(lhs_value, rhs_value);
        }
        default: {
            throw LogicException("This should never happen");
        }
        }
    }
    case ObjectGenType::List: {
        std::vector<ObjectId> lhs_list = Conversions::unpack_list(lhs);
        std::vector<ObjectId> rhs_list = Conversions::unpack_list(rhs);
        if (lhs_list == rhs_list) {
            return 0;
        }
        return lhs_unmasked_id - rhs_unmasked_id;
    }
    case ObjectGenType::Dict: {
        auto lhs_dict = Common::Conversions::unpack_dictionary(lhs);
        auto rhs_dict = Common::Conversions::unpack_dictionary(rhs);
        Dictionary& lhs_ref(*lhs_dict);
        Dictionary& rhs_ref(*rhs_dict);
        if (lhs_ref == rhs_ref) {
            return 0;
        }
        return lhs_unmasked_id - rhs_unmasked_id;
    }
    default: {
        auto lhs_t = lhs.type();
        auto rhs_t = lhs.type();
        if (lhs_t == rhs_t) {
            return lhs_unmasked_id - rhs_unmasked_id;
        } else {
            return static_cast<int64_t>(lhs_t) - static_cast<int64_t>(rhs_t);
        }
    }
    }
}
