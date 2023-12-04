#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vector>

namespace crude_engine
{

class Device;

struct Descriptor_Set_Layout_Create_Info
{
  std::shared_ptr<const Device>              device;
  std::vector<VkDescriptorSetLayoutBinding>  bindings;
  explicit Descriptor_Set_Layout_Create_Info(std::shared_ptr<const Device>                     device,
                                             const std::vector<VkDescriptorSetLayoutBinding>&  bindings);
};

class Descriptor_Set_Layout : public TObject<VkDescriptorSetLayout>
{
public:
  explicit Descriptor_Set_Layout(const Descriptor_Set_Layout_Create_Info& createInfo);
  ~Descriptor_Set_Layout();
private:
  std::shared_ptr<const Device>  m_device;
};

}
