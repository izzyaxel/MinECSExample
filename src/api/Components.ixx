module;

#include <cstdint>
#include <type_traits>
#include <cstdio>
#include <typeinfo>

export module Components;

import Memory;

export struct TransformComponent
{

};

export template <typename T>
concept ValidComponent = !std::is_integral_v<T> && !std::is_array_v<T>;

template <ValidComponent T>
SlabAllocator<T>* COMPONENT_ALLOCATOR = nullptr;

export namespace Components
{
  template<ValidComponent T>
  auto printAllocatorPtr() -> void
  {
    printf("%s: %p\n", typeid(T).name(), (void*)COMPONENT_ALLOCATOR<T>);
  }

  template <ValidComponent T>
  auto createAllocator(const uint32_t maxComponents) -> void
  {
    printf("Engine scope: ");
    COMPONENT_ALLOCATOR<T> = new SlabAllocator<T>(maxComponents);
    printAllocatorPtr<T>();
  }

  template <ValidComponent T>
  auto destroyAllocator() -> void
  {
    delete COMPONENT_ALLOCATOR<T>;
  }
}
