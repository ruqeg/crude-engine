#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include <vector>
#include <array>

namespace crude_vulkan_01
{

class Color_Blend_State_Create_Info : public VkPipelineColorBlendStateCreateInfo
{
public:
  explicit Color_Blend_State_Create_Info(const std::vector<VkPipelineColorBlendAttachmentState>&  attachments,
                                         const std::array<float32, 4>&                            blendConstants,
                                         VkBool32                                                 logicOpEnable,
                                         VkLogicOp                                                logicOp);
  Color_Blend_State_Create_Info(const Color_Blend_State_Create_Info& other);
  Color_Blend_State_Create_Info(Color_Blend_State_Create_Info&& other) noexcept;
  Color_Blend_State_Create_Info& operator=(const Color_Blend_State_Create_Info& other);
  Color_Blend_State_Create_Info& operator=(Color_Blend_State_Create_Info&& other) noexcept;
  ~Color_Blend_State_Create_Info();
private:
  void copy(const VkPipelineColorBlendAttachmentState*  pAttachments,
            uint32                                      attachmentsCount,
            const float32*                              pBlendConstants,
            VkBool32                                    logicOpEnable,
            VkLogicOp                                   logicOp);
  void move(Color_Blend_State_Create_Info& other);
};

}
