#include <gameengine.hh>
#include <cstdio>

import Components;

int main()
{
  Engine::createAllocator();

  //TODO FIXME this prints nullptr because the templated allocator is not instantiated inside the Components module,
  // it's instantiated in gameengine.cc, where createAllocator is called
  printf("Game scope: ");
  Components::printAllocatorPtr<TransformComponent>();

  Engine::destroyAllocator();
  return 0;
}