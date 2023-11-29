#include "input_assembly_state_create_info.hpp"

namespace crude_vulkan_01 
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
