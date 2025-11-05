#include "api/gameengine.hh"

import Components;

namespace Engine
{
  auto createAllocator() -> void
  {
    Components::createAllocator<TransformComponent>(10);
  }

  auto destroyAllocator() -> void
  {
    Components::destroyAllocator<TransformComponent>();
  }
}
