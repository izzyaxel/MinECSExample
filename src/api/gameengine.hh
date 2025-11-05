#pragma once

#include <typeinfo>
#include <unordered_map>

namespace Engine
{
  auto createAllocator() -> void;
  auto destroyAllocator() -> void;
}
