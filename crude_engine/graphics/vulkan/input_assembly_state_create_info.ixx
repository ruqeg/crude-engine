module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.input_assembly_state_create_info;

export namespace crude_engine 
{

class Input_Assembly_State_Create_Info : public VkPipelineInputAssemblyStateCreateInfo
{
public:
  explicit Input_Assembly_State_Create_Info(VkPrimitiveTopology  topology,
                                            VkBool32             primitiveRestartEnable);
};

}
