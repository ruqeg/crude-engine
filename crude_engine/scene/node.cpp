#include <typeindex>
#include <flecs.h>

module crude.scene.node;

import crude.scene.scene;

namespace crude::scene
{

Node::Node(core::shared_ptr<Scene> scene, const char* name)
  : m_entity(scene->getWorld(), name)
{}

}