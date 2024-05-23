module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.input_assembly_state_create_info;

export namespace crude::graphics
{

class Input_Assembly_State_Create_Info : public VkPipelineInputAssemblyStateCreateInfo
{
public:
  explicit Input_Assembly_State_Create_Info(VkPrimitiveTopology  topology,
                                            VkBool32             primitiveRestartEnable);
};

}
