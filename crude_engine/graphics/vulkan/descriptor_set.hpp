#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vector>

namespace crude_engine
{

class Device;
class Descriptor_Set_Layout;
class Descriptor_Pool;

struct Descriptor_Set_Create_Info
{
  std::shared_ptr<const Device>                        device;
  std::vector<std::shared_ptr<Descriptor_Set_Layout>>  setLayouts;
  std::shared_ptr<Descriptor_Pool>                     pool;
  explicit Descriptor_Set_Create_Info(std::shared_ptr<const Device>                               device,
                                      const std::vector<std::shared_ptr<Descriptor_Set_Layout>>&  setLayouts,
                                      std::shared_ptr<Descriptor_Pool>                            pool);
};

class Descriptor_Set : public TObject<VkDescriptorSet>
{
public:
  explicit Descriptor_Set(const Descriptor_Set_Create_Info& createInfo);
  ~Descriptor_Set();
private:
  std::shared_ptr<const Device>                        m_device;
  std::vector<std::shared_ptr<Descriptor_Set_Layout>>  m_setLayouts;
  std::shared_ptr<Descriptor_Pool>                     m_pool;
};

}
