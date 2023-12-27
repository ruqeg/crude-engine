#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vector>

namespace crude_engine
{

class Device;
class Descriptor_Set_Layout;

struct Pipeline_Layout_Create_Info
{
  std::shared_ptr<const Device>                              device;
  std::vector<std::shared_ptr<const Descriptor_Set_Layout>>  descriptorSetLayouts;
  std::vector<VkPushConstantRange>                           pushConstantRanges;
  explicit Pipeline_Layout_Create_Info(std::shared_ptr<const Device>                                     device,
                                       const std::vector<std::shared_ptr<const Descriptor_Set_Layout>>&  descriptorSetLayouts,
                                       const std::vector<VkPushConstantRange>&                           pushConstantRanges);
};

class Pipeline_Layout : public TObject<VkPipelineLayout>
{
public:
  explicit Pipeline_Layout(const Pipeline_Layout_Create_Info& createInfo);
  ~Pipeline_Layout();
private:
  std::shared_ptr<const Device>  m_device;
};

}