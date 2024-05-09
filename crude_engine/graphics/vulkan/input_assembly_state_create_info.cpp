#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.input_assembly_state_create_info;

namespace crude_engine 
{
  
Input_Assembly_State_Create_Info::Input_Assembly_State_Create_Info(VkPrimitiveTopology  topology,
                                                                   VkBool32             primitiveRestartEnable)
{
  this->sType                   = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  this->pNext                   = nullptr;
  this->flags                   = 0u;
  this->topology                = topology;
  this->primitiveRestartEnable  = primitiveRestartEnable;
}

}
