module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.structure_chain;


export namespace crude::gfx::vk
{

class Structure_Chain
{
public:
  Structure_Chain();
  VkBaseOutStructure* getHeadNode();
  const VkBaseOutStructure* getHeadNode() const;
  template<class T>
  void linkNode(T& node);
private:
  void linkNodeBase(VkBaseOutStructure* baseNode);
private:
  VkBaseOutStructure* m_head;
  VkBaseOutStructure* m_tail;
  
};

template<class T>
void Structure_Chain::linkNode(T& node)
{
  linkNodeBase(reinterpret_cast<VkBaseOutStructure*>(&node));
}

}
