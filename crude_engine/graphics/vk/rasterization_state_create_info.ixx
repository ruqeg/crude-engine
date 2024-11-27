module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.rasterization_state_create_info;

export namespace crude::gfx::vk
{

class Rasterization_State_Create_Info : public VkPipelineRasterizationStateCreateInfo
{
public:
  struct Initialize
  {
    VkBool32         depthClampEnable;
    VkBool32         rasterizerDiscardEnable;
    VkPolygonMode    polygonMode;
    VkCullModeFlags  cullMode;
    VkFrontFace      frontFace;
    VkBool32         depthBiasEnable;
    float            depthBiasConstantFactor;
    float            depthBiasClamp;
    float            depthBiasSlopeFactor;
    float            lineWidth;
  };
public:
  explicit Rasterization_State_Create_Info(const Initialize& info)
  {
    this->sType                     = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    this->flags                     = 0u;
    this->pNext                     = nullptr;

    this->depthClampEnable          = info.depthClampEnable;
    this->rasterizerDiscardEnable   = info.rasterizerDiscardEnable;
    this->polygonMode               = info.polygonMode;
    this->cullMode                  = info.cullMode;
    this->frontFace                 = info.frontFace;
    this->depthBiasEnable           = info.depthBiasEnable;
    this->depthBiasConstantFactor   = info.depthBiasConstantFactor;
    this->depthBiasClamp            = info.depthBiasClamp;
    this->depthBiasSlopeFactor      = info.depthBiasSlopeFactor;
    this->lineWidth                 = info.lineWidth;
  }
};

}
