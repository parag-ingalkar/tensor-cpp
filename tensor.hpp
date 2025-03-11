
#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>
#include <utility>
// #include <concepts>

template <class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic ComponentType>
class Tensor
{
public:
    // Constructs a tensor with rank = 0 and zero-initializes the element.
    Tensor() : shape_(), numElements_(1)
    {
        data_ = std::make_unique<ComponentType[]>(1);
        data_[0] = ComponentType{0};
    }

    // Constructs a tensor with arbitrary shape and zero-initializes all elements.
    Tensor(const std::vector<size_t> &shape) : shape_(shape)
    {
        numElements_ = calculateNumElements(shape_);
        data_ = std::make_unique<ComponentType[]>(numElements_);
        std::fill(data_.get(), data_.get() + numElements_, ComponentType{0});
    }

    // Constructs a tensor with arbitrary shape and fills it with the specified value.
    explicit Tensor(const std::vector<size_t> &shape, const ComponentType &fillValue) : shape_(shape)
    {
        numElements_ = calculateNumElements(shape_);
        data_ = std::make_unique<ComponentType[]>(numElements_);
        std::fill(data_.get(), data_.get() + numElements_, fillValue);
    }

    // Copy-constructor.
    Tensor(const Tensor<ComponentType> &other) : shape_(other.shape_), numElements_(other.numElements_)
    {
        data_ = std::make_unique<ComponentType[]>(numElements_);
        std::copy(other.data_.get(), other.data_.get() + other.numElements_, data_.get());
    }

    // Move-constructor.
    Tensor(Tensor<ComponentType> &&other) noexcept : data_(std::exchange(other.data_, std::make_unique<ComponentType[]>(1))), shape_(std::exchange(other.shape_, std::vector<size_t>())), numElements_(std::exchange(other.numElements_, 1))
    {
        other.data_[0] = ComponentType{0};
    }

    // Copy-assignment
    Tensor &
    operator=(const Tensor<ComponentType> &other)
    {
        if (this != &other)
        {
            shape_ = other.shape_;
            numElements_ = other.numElements_;
            data_ = std::make_unique<ComponentType[]>(numElements_);
            std::copy(other.data_.get(), other.data_.get() + other.numElements_, data_.get());
        }
        return *this;
    }

    // Move-assignment
    Tensor &
    operator=(Tensor<ComponentType> &&other) noexcept
    {
        if (this != &other)
        {
            data_ = std::exchange(other.data_, std::make_unique<ComponentType[]>(1));
            shape_ = std::exchange(other.shape_, std::vector<size_t>());
            numElements_ = std::exchange(other.numElements_, 1);
            other.data_[0] = ComponentType{0};
        }
        return *this;
    }

    // Destructor
    ~Tensor() = default;

    // Returns the rank of the tensor.
    [[nodiscard]] size_t rank() const
    {
        return shape_.size();
    }

    // Returns the shape of the tensor.
    [[nodiscard]] std::vector<size_t> shape() const
    {
        return shape_;
    }

    // Returns the number of elements of this tensor.
    [[nodiscard]] size_t numElements() const
    {
        return numElements_;
    }

    // Element access function
    const ComponentType &
    operator()(const std::vector<size_t> &idx) const
    {
        return data_[calculateLinearIndex(idx)];
    }

    // Element mutation function
    ComponentType &
    operator()(const std::vector<size_t> &idx)
    {
        return data_[calculateLinearIndex(idx)];
    }

    // Non-const version for mutating access
    ComponentType &value(size_t idx)
    {
        if (idx >= numElements_)
        {
            throw std::out_of_range("Linear index out of bounds");
        }
        return data_[idx];
    }

    // Const version for read-only access
    const ComponentType &value(size_t idx) const
    {
        if (idx >= numElements_)
        {
            throw std::out_of_range("Linear index out of bounds");
        }
        return data_[idx];
    }

private:
    // TODO: Probably you need some members here...
    std::unique_ptr<ComponentType[]> data_;
    std::vector<size_t> shape_;
    size_t numElements_;

    size_t calculateNumElements(const std::vector<size_t> &shape)
    {
        if (shape.empty())
        {
            return 1;
        }

        return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<size_t>());
    }

    size_t calculateLinearIndex(const std::vector<size_t> &idx) const
    {
        if (idx.size() != rank())
        {
            if (rank() == 0 && idx.empty())
            {
                return 0;
            }
            throw std::invalid_argument("Index dimension mismatch");
        }

        size_t linearIndex = 0;
        size_t stride = 1;

        for (int i = static_cast<int>(rank()) - 1; i >= 0; --i)
        {
            if (idx[i] >= shape_[i])
            {
                throw std::out_of_range("Index out of bounds");
            }
            linearIndex += idx[i] * stride;
            stride *= shape_[i];
        }

        return linearIndex;
    }
};

// TODO: Implement all methods of the Tensor class template.

// Returns true if the shapes and all elements of both tensors are equal.
template <Arithmetic ComponentType>
bool operator==(const Tensor<ComponentType> &a, const Tensor<ComponentType> &b)
{
    // TODO: Implement this comparison.
    if (a.rank() != b.rank())
    {
        return false;
    }
    if (a.shape() != b.shape())
    {
        return false;
    }

    for (size_t i = 0; i < a.numElements(); i++)
    {
        if (a.value(i) != b.value(i))
        {
            return false;
        }
    }
    return true;
}

// Pretty-prints the tensor to stdout.
// This is not necessary (and not covered by the tests) but nice to have, also for debugging (and for exercise of course...).
template <Arithmetic ComponentType>
std::ostream &
operator<<(std::ostream &out, const Tensor<ComponentType> &tensor)
{
    // TODO (optional): Implement some nice stdout printer for debugging/exercise.
}

// Reads a tensor from file.
template <Arithmetic ComponentType>
Tensor<ComponentType> readTensorFromFile(const std::string &filename)
{
    // TODO: Implement this function to read in tensors from file.
    //       The format is defined in the instructions.

    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    size_t rank;
    file >> rank;

    std::vector<size_t> shape(rank);
    for (size_t i = 0; i < rank; i++)
    {
        file >> shape[i];
    }

    Tensor<ComponentType> tensor(shape);

    for (size_t i = 0; i < tensor.numElements(); i++)
    {
        file >> tensor.value(i);
    }
    return tensor;
}

// Writes a tensor to file.
template <Arithmetic ComponentType>
void writeTensorToFile(const Tensor<ComponentType> &tensor, const std::string &filename)
{
    // TODO: Implement this function to write tensors to file.
    //       The format is defined in the instructions.

    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Could not open file for writing : " + filename);
    }

    file << tensor.rank() << "\n";

    for (size_t i = 0; i < tensor.rank(); i++)
    {
        file << tensor.shape()[i] << "\n";
    }

    for (size_t i = 0; i < tensor.numElements(); i++)
    {
        file << tensor.value(i) << "\n";
    }
}
