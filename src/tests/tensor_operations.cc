#include <cassert>
#include <iostream>
#include <string>

#include "graph_models/common/datatypes/tensor.h"

typedef bool TestFunction();

template<typename T>
bool from_literal_test()
{
    const std::string input = "[      1, 1.0, 1e-2, 1e-5, -0, -0.0, 0, 0.0, 00.00, 1e30, 3.14159 ]";
    const std::vector<T> expected = { 1, 1.0, 1e-2, 1e-5, -0, -0.0, 0, 0.0, 00.00, 1e30, 3.14159 };

    bool error;
    const auto tensor = Tensor<T>::from_literal(input, &error);
    assert(!error);
    if (tensor.size() != expected.size()) {
        std::cout << "Tensors size mismatch: " << tensor.size() << " != " << expected.size();
        return false;
    }

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        if (tensor[i] != expected[i]) {
            std::cout << "Tensor value mistmatch at index " << i << ": " << tensor[i]
                      << " != " << expected[i];
            return false;
        }
    }

    return true;
}

template<typename T>
bool multiplication_test()
{
    const std::string lhs_tensor_str = "[0, 0, 1, -1, 0.1, 2,  9]";
    const std::string rhs_tensor_str = "[0, 1, 0,  1,   1, 4,  9]";
    const std::string res_tensor_str = "[0, 0, 0, -1, 0.1, 8, 81]";

    bool error;
    auto lhs = Tensor<T>::from_literal(lhs_tensor_str, &error);
    assert(!error);
    const auto rhs = Tensor<T>::from_literal(rhs_tensor_str, &error);
    assert(!error);
    const auto res = Tensor<T>::from_literal(res_tensor_str, &error);
    assert(!error);

    auto lhs_copy = lhs;
    lhs.multiply(rhs);
    if (lhs != res) {
        std::cout << "(" << lhs_copy << " * " << rhs << ")\ngot\n" << lhs << "\nwant\n" << res << "\n";
        return false;
    }

    return true;
}

template<typename T>
bool division_test()
{
    const std::string lhs_tensor_str = "[-1, -0.1, 0,  0.1,   1,   2, 10, 100]";
    const std::string rhs_tensor_str = "[ 1,  0.1, 1,   -1,   2,   4, 10,   2]";
    const std::string res_tensor_str = "[-1,   -1, 0, -0.1, 0.5, 0.5,  1,  50]";

    bool error;
    auto lhs = Tensor<T>::from_literal(lhs_tensor_str, &error);
    assert(!error);
    const auto rhs = Tensor<T>::from_literal(rhs_tensor_str, &error);
    assert(!error);
    const auto res = Tensor<T>::from_literal(res_tensor_str, &error);
    assert(!error);

    auto lhs_copy = lhs;
    lhs.divide(rhs);
    if (lhs != res) {
        std::cout << "(" << lhs_copy << " / " << rhs << ")\ngot\n" << lhs << "\nwant\n" << res << "\n";
        return false;
    }

    return true;
}

template<typename T>
bool addition_test()
{
    const std::string lhs_tensor_str = "[-1, -0.1,  0, 2.5, 1, 100]";
    const std::string rhs_tensor_str = "[ 1,  0.1,  1, 0.5, 2,   2]";
    const std::string res_tensor_str = "[ 0,    0,  1, 3.0, 3, 102]";

    bool error;
    auto lhs = Tensor<T>::from_literal(lhs_tensor_str, &error);
    assert(!error);
    const auto rhs = Tensor<T>::from_literal(rhs_tensor_str, &error);
    assert(!error);
    const auto res = Tensor<T>::from_literal(res_tensor_str, &error);
    assert(!error);

    auto lhs_copy = lhs;
    lhs.add(rhs);
    if (lhs != res) {
        std::cout << "(" << lhs_copy << " / " << rhs << ")\ngot\n" << lhs << "\nwant\n" << res << "\n";
        return false;
    }

    return true;
}

template<typename T>
bool subtraction_test()
{
    const std::string lhs_tensor_str = "[-1, -0.1,  0, 2.5,  1, 100]";
    const std::string rhs_tensor_str = "[ 1,  0.1,  1, 0.5,  2,   2]";
    const std::string res_tensor_str = "[-2, -0.2, -1, 2.0, -1,  98]";

    bool error;
    auto lhs = Tensor<T>::from_literal(lhs_tensor_str, &error);
    assert(!error);
    const auto rhs = Tensor<T>::from_literal(rhs_tensor_str, &error);
    assert(!error);
    const auto res = Tensor<T>::from_literal(res_tensor_str, &error);
    assert(!error);

    auto lhs_copy = lhs;
    lhs.subtract(rhs);
    if (lhs != res) {
        std::cout << "(" << lhs_copy << " / " << rhs << ")\ngot\n" << lhs << "\nwant\n" << res << "\n";
        return false;
    }

    return true;
}

template<typename T>
bool metrics_test()
{
    const std::string tensor_str = "[1, 1]";

    bool error;
    const auto tensor = Tensor<T>::from_literal(tensor_str, &error);
    assert(!error);

    // Self cosine equals 1
    const auto zero_cosine = tensor.cosine_similarity(tensor);
    if (zero_cosine != T(1)) {
        std::cout << "zero_cosine = " << zero_cosine;
        return false;
    }
    // Opposite cosine equals -1
    const std::string tensor_opposite_str = "[-1, -1]";
    const auto tensor_opposite = Tensor<T>::from_literal(tensor_opposite_str, &error);
    assert(!error);
    const auto cosine_opposite = tensor.cosine_similarity(tensor_opposite);
    if (cosine_opposite != T(-1)) {
        std::cout << "cosine_opposite = " << cosine_opposite;
        return false;
    }
    // Orthogonal cosine equals 0
    const std::string tensor_orthogonal_str = "[-1, 1]";
    const auto tensor_orthogonal = Tensor<T>::from_literal(tensor_orthogonal_str, &error);
    assert(!error);
    const auto cosine_ortho = tensor.cosine_similarity(tensor_orthogonal);
    if (cosine_ortho != T(0)) {
        std::cout << "cosine_ortho = " << cosine_ortho;
        return false;
    }

    // Self distance equals zero
    const auto zero_euclidean = tensor.euclidean_distance(tensor);
    if (zero_euclidean != T(0)) {
        return false;
    }
    const auto zero_manhattan = tensor.manhattan_distance(tensor);
    if (zero_manhattan != T(0)) {
        return false;
    }

    return true;
}

int main()
{
    std::vector<TestFunction*> tests = {
        &from_literal_test<float>,    &from_literal_test<double>, &multiplication_test<float>,
        &multiplication_test<double>, &division_test<float>,      &division_test<double>,
        &addition_test<float>,        &addition_test<double>,     &subtraction_test<float>,
        &subtraction_test<double>,    &metrics_test<float>,       &metrics_test<double>,
    };

    for (const auto& test_func : tests) {
        if (!test_func()) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}