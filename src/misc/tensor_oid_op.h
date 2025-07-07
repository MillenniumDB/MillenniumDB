#pragma once

#include "graph_models/common/conversions.h"
#include "graph_models/object_id.h"

namespace misc {
// helper functions to handle tensor object id operations
// it is assumed that at least one of the operands is a tensor

template<
    typename T,
    typename ToScalarFunc,
    typename TensorTensorOp,
    typename TensorScalarOp,
    typename ScalarTensorOp>
inline ObjectId op_oid_tensor(
    ObjectId lhs_oid,
    ObjectId rhs_oid,
    ToScalarFunc to_scalar_func,
    TensorTensorOp tensor_tensor_op,
    TensorScalarOp tensor_scalar_op,
    ScalarTensorOp scalar_tensor_op
)
{
    const auto lhs_generic_type = lhs_oid.id & ObjectId::GENERIC_TYPE_MASK;
    const auto rhs_generic_type = rhs_oid.id & ObjectId::GENERIC_TYPE_MASK;

    if (lhs_generic_type == ObjectId::MASK_TENSOR) {
        auto lhs = Common::Conversions::to_tensor<T>(lhs_oid);
        if (rhs_generic_type == ObjectId::MASK_TENSOR) {
            auto rhs = Common::Conversions::to_tensor<T>(rhs_oid);
            tensor_tensor_op(lhs, rhs);
        } else {
            const T rhs = to_scalar_func(rhs_oid);
            tensor_scalar_op(lhs, rhs);
        }
        return Common::Conversions::pack_tensor(lhs);
    } else {
        assert(rhs_generic_type == ObjectId::MASK_TENSOR && "at least one operand must be a tensor");
        auto rhs = Common::Conversions::to_tensor<T>(rhs_oid);
        const T lhs = to_scalar_func(lhs_oid);
        scalar_tensor_op(lhs, rhs);
        return Common::Conversions::pack_tensor(rhs);
    }
}

template<typename T, typename ToScalarFunc>
inline ObjectId add_oid_tensor(ObjectId lhs, ObjectId rhs, ToScalarFunc to_scalar_func)
{
    return op_oid_tensor<T>(
        lhs,
        rhs,
        to_scalar_func,
        [](auto& lhs_tensor, const auto& rhs_tensor) { lhs_tensor.inplace_add_tensor(rhs_tensor); },
        [](auto& lhs_tensor, T rhs_scalar) { lhs_tensor.inplace_add_scalar(rhs_scalar); },
        [](T lhs_scalar, auto& rhs_tensor) { rhs_tensor.inplace_add_scalar(lhs_scalar); }
    );
}

template<typename T, typename ToScalarFunc>
inline ObjectId sub_oid_tensor(ObjectId lhs, ObjectId rhs, ToScalarFunc to_scalar_func)
{
    return op_oid_tensor<T>(
        lhs,
        rhs,
        to_scalar_func,
        [](auto& lhs_tensor, const auto& rhs_tensor) { lhs_tensor.inplace_sub_tensor(rhs_tensor); },
        [](auto& lhs_tensor, T rhs_scalar) { lhs_tensor.inplace_sub_scalar_rhs(rhs_scalar); },
        [](T lhs_scalar, auto& rhs_tensor) { rhs_tensor.inplace_sub_scalar_lhs(lhs_scalar); }
    );
}

template<typename T, typename ToScalarFunc>
inline ObjectId mul_oid_tensor(ObjectId lhs, ObjectId rhs, ToScalarFunc to_scalar_func)
{
    return op_oid_tensor<T>(
        lhs,
        rhs,
        to_scalar_func,
        [](auto& lhs_tensor, const auto& rhs_tensor) { lhs_tensor.inplace_mul_tensor(rhs_tensor); },
        [](auto& lhs_tensor, T rhs_scalar) { lhs_tensor.inplace_mul_scalar(rhs_scalar); },
        [](T lhs_scalar, auto& rhs_tensor) { rhs_tensor.inplace_mul_scalar(lhs_scalar); }
    );
}

template<typename T, typename ToScalarFunc>
inline ObjectId div_oid_tensor(ObjectId lhs, ObjectId rhs, ToScalarFunc to_scalar_func)
{
    return op_oid_tensor<T>(
        lhs,
        rhs,
        to_scalar_func,
        [](auto& lhs_tensor, const auto& rhs_tensor) { lhs_tensor.inplace_div_tensor(rhs_tensor); },
        [](auto& lhs_tensor, T rhs_scalar) { lhs_tensor.inplace_div_scalar_rhs(rhs_scalar); },
        [](T lhs_scalar, auto& rhs_tensor) { rhs_tensor.inplace_div_scalar_lhs(lhs_scalar); }
    );
}

} // namespace misc
