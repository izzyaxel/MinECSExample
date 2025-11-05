module;

#include <unordered_map>
#include <cstdint>
#include <cstdlib>

export module Memory;

export template <typename T>
struct loan_ptr
{
  loan_ptr() = default;

  loan_ptr(T* ptr) : ptr(ptr) {}

  loan_ptr(const loan_ptr& other) : ptr(other.ptr)
  {
    if(this != &other)
    {
      this->ptr = other.ptr;
    }
  }

  auto operator=(const loan_ptr& other) -> loan_ptr&
  {
    if(this != &other)
    {
      this->ptr = other.ptr;
    }
    return *this;
  }

  explicit operator bool() const
  {
    return this->ptr != nullptr;
  }

  auto operator==(const loan_ptr& other) -> bool
  {
    return this->get() == other.get();
  }

  auto operator*() const noexcept -> T&
  {
    return *this->ptr;
  }

  auto operator->() const noexcept -> T*
  {
    return this->ptr;
  }

  constexpr auto empty() const noexcept -> bool
  {
    return this->ptr == nullptr;
  }

  constexpr auto get() const noexcept -> T*
  {
    return this->ptr;
  }

  constexpr auto reset() -> void
  {
    this->ptr = nullptr;
  }

private:
  T* ptr = nullptr;
};

export template <typename T>
using LP = loan_ptr<T>;

export template <typename T>
struct SlabAllocator
{
  SlabAllocator() = default;

  explicit SlabAllocator(const uint16_t preallocatedSlabs)
  {
    const size_t dataSize = preallocatedSlabs > 1 ? preallocatedSlabs : 1;
    this->memory = static_cast<T*>(calloc(dataSize, T_SIZE));
    this->elementCapacity = dataSize;
    for(size_t i = 0; i < this->elementCapacity; ++i)
    {
      this->allocMap[i] = false;
    }
  }

  ~SlabAllocator()
  {
    if(this->memory)
    {
      free(this->memory);
    }
  }

  template <typename ...Args>
  auto getSlab(Args... args) -> LP<T>
  {
    if(this->elements + 1 > this->elementCapacity)
    {
      return nullptr;
    }

    T* slab = this->firstFree();
    if(!slab)
    {
      return nullptr;
    }

    slab = new T(args...);
    const size_t index = this->getIndex(slab);
    this->allocMap[index] = true;
    ++this->elements;
    return LP<T>(slab);
  }

  auto freeSlab(LP<T>& slab) -> void
  {
    if(slab.empty())
    {
      return;
    }

    const size_t index = this->getIndex(slab.get());
    *slab = {};
    slab.reset();
    --this->elements;
    this->allocMap.at(index) = false;
  }

private:
  static constexpr size_t T_SIZE = sizeof(T);
  size_t elementCapacity = 0;
  size_t elements = 0;
  T* memory = nullptr;
  std::unordered_map<uint64_t, bool> allocMap{};

  auto getIndex(const T* slab) -> size_t
  {
    return (slab - this->memory) / T_SIZE;
  }

  auto firstFree() -> T*
  {
    for(size_t i = 0; i < this->elementCapacity; ++i)
    {
      if(!this->allocMap.at(i))
      {
        return this->memory + i * T_SIZE;
      }
    }
    return nullptr;
  }
};
