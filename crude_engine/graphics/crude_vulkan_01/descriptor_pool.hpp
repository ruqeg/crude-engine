#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vector>

namespace crude_vulkan_01
{

class Device;

struct Descriptor_Pool_Create_Info
{
  std::shared_ptr<const Device>      device;
  std::vector<VkDescriptorPoolSize>  poolSizes;
  uint32                             maxSets;
  bool                               freeDescriptorSet;
  explicit Descriptor_Pool_Create_Info(std::shared_ptr<const Device>             device,
                                       const std::vector<VkDescriptorPoolSize>&  poolSizes,
                                       uint32                                    maxSets,
                                       bool                                      freeDescriptorSet = false);
};

class Descriptor_Pool : public TObject<VkDescriptorPool>
{
public:
  explicit Descriptor_Pool(const Descriptor_Pool_Create_Info& createInfo);
  ~Descriptor_Pool();
  bool canFreeDescriptorSet() const;
private:
  std::shared_ptr<const Device>  m_device;
  bool                           m_freeDescriptorSet;
};

}
