#include <gameengine.hh>
#include <cstdio>

import Components;

int main()
{
  Engine::createAllocator();

  printf("Game scope: ");
  Components::printAllocatorPtr<TransformComponent>();

  Engine::destroyAllocator();
  return 0;
}