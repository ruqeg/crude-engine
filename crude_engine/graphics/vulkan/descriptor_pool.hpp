#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Device;

class Descriptor_Pool : public TObject<VkDescriptorPool>
{
public:
  explicit Descriptor_Pool(Shared_Ptr<const Device>  device,
                           VkDescriptorPoolSize*     pPoolSizes,
                           uint32                    poolSizeCount,
                           uint32                    maxSets,
                           bool                      freeDescriptorSet);
  ~Descriptor_Pool();
  bool canFreeDescriptorSet() const;
private:
  const Shared_Ptr<const Device>  m_device;
  const bool                      m_freeDescriptorSet;
};

}
