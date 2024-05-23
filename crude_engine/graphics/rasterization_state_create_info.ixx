module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.rasterization_state_create_info;

export namespace crude::graphics
{

class Rasterization_State_Create_Info : public VkPipelineRasterizationStateCreateInfo
{
public:
  explicit Rasterization_State_Create_Info(VkBool32         depthClampEnable,
                                           VkBool32         rasterizerDiscardEnable,
                                           VkPolygonMode    polygonMode,
                                           VkCullModeFlags  cullMode,
                                           VkFrontFace      frontFace,
                                           VkBool32         depthBiasEnable,
                                           float            depthBiasConstantFactor,
                                           float            depthBiasClamp,
                                           float            depthBiasSlopeFactor,
                                           float            lineWidth);
};

}
