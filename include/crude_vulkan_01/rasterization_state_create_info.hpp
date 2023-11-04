#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01
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
