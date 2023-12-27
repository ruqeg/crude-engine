#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"

namespace crude_engine
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