#pragma once

#include "tensor.hpp"

template <typename ComponentType>
class Vector
{
public:
    // Default-constructor.
    Vector() = default;

    // Constructor for vector of certain size.
    explicit Vector(size_t size) : tensor_({size}) {}

    // Constructor for vector of certain size with constant fill-value.
    Vector(size_t size, const ComponentType &fillValue) : tensor_({size}, fillValue) {}

    // Constructing vector from file.
    Vector(const std::string &filename) : tensor_(readTensorFromFile<ComponentType>(filename))
    {
        if (tensor_.rank() != 1)
        {
            throw std::runtime_error("File does not contain valid Vector (Rank must be equal to 1)");
        }
    }

    // Number of elements in this vector.
    [[nodiscard]] size_t size() const
    {
        return tensor_.numElements();
    }

    // Element access function
    const ComponentType &
    operator()(size_t idx) const
    {
        return tensor_({idx});
    }

    // Element mutation function
    ComponentType &
    operator()(size_t idx)
    {
        return tensor_({idx});
    }

    // Reference to internal tensor.
    Tensor<ComponentType> &tensor()
    {
        return tensor_;
    }

private:
    Tensor<ComponentType> tensor_;
};

template <typename ComponentType>
class Matrix
{
public:
    // Default-constructor.
    Matrix() = default;

    // Constructor for matrix of certain size.
    explicit Matrix(size_t rows, size_t cols) : tensor_({rows, cols}) {}

    // Constructor for matrix of certain size with constant fill-value.
    Matrix(size_t rows, size_t cols, const ComponentType &fillValue) : tensor_({rows, cols}, fillValue) {}

    // Constructing matrix from file.
    Matrix(const std::string &filename) : tensor_(readTensorFromFile<ComponentType>(filename))
    {
        if (tensor_.rank() != 2)
        {
            throw std::runtime_error("File does not contain valid Matrix (Rank must be equal to 2)");
        }
    }

    // Number of rows.
    [[nodiscard]] size_t rows() const
    {
        return tensor_.shape()[0];
    }

    // Number of columns.
    [[nodiscard]] size_t cols() const
    {
        return tensor_.shape()[1];
    }

    // Element access function
    const ComponentType &
    operator()(size_t row, size_t col) const
    {
        return tensor_({row, col});
    }

    // Element mutation function
    ComponentType &
    operator()(size_t row, size_t col)
    {
        return tensor_({row, col});
    }

    // Reference to internal tensor.
    Tensor<ComponentType> &tensor()
    {
        return tensor_;
    }

private:
    Tensor<ComponentType> tensor_;
};

// TODO: Implement all methods.

// Performs a matrix-vector multiplication.
template <typename ComponentType>
Vector<ComponentType> matvec(const Matrix<ComponentType> &mat, const Vector<ComponentType> &vec)
{
    // TODO: Implement this.

    size_t mat_rows = mat.rows(), mat_cols = mat.cols();
    size_t vec_rows = vec.size();

    if (mat_cols != vec_rows)
    {
        throw std::runtime_error("Dimension mismatch : Matrix-Vector multiplication not defined.");
    }

    Vector<ComponentType> resultant_vector(mat_rows, ComponentType(0));
    for (size_t i = 0; i < mat_rows; i++)
    {
        for (size_t j = 0; j < mat_cols; j++)
        {
            resultant_vector(i) += mat(i, j) * vec(j);
        }
    }
    return resultant_vector;
}
