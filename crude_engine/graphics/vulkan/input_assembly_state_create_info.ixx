module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.input_assembly_state_create_info;

export namespace crude::gfx::vk
{

class Input_Assembly_State_Create_Info : public VkPipelineInputAssemblyStateCreateInfo
{
public:
  struct Initialize
  {
    VkPrimitiveTopology  topology;
    VkBool32             primitiveRestartEnable;
  };
public:
  explicit Input_Assembly_State_Create_Info(const Initialize& info)
  {
    this->sType                   = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    this->pNext                   = nullptr;
    this->flags                   = 0u;
    this->topology                = info.topology;
    this->primitiveRestartEnable  = info.primitiveRestartEnable;
  }
};

}
