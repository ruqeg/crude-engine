#include <vulkan/vulkan.h>

module crude.gfx.vk.acceleration_structure_geometry_triangles;

import crude.gfx.vk.acceleration_structure_input_buffer;
import crude.gfx.vk.constants;
import crude.gfx.vk.utils;
import crude.gfx.vk.format;

namespace crude::gfx::vk
{

Acceleration_Structure_Geometry_Triangles::Acceleration_Structure_Geometry_Triangles()
  : primitiveCount{ 0u }
{
  this->sType                                          = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
  this->pNext                                          = nullptr;
  this->geometryType                                   = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
  this->flags                                          = 0u;

  this->geometry.triangles.sType                       = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
  this->geometry.triangles.pNext                       = nullptr;
  this->geometry.triangles.vertexFormat                = VK_FORMAT_UNDEFINED;
  this->geometry.triangles.vertexData.deviceAddress    = cDeviceAddressNull;
  this->geometry.triangles.vertexStride                = 0u;
  this->geometry.triangles.maxVertex                   = 0u;
  this->geometry.triangles.indexType                   = VK_INDEX_TYPE_NONE_KHR;
  this->geometry.triangles.indexData.deviceAddress     = cDeviceAddressNull;
  this->geometry.triangles.transformData.deviceAddress = cDeviceAddressNull;
}

Acceleration_Structure_Geometry_Triangles::Acceleration_Structure_Geometry_Triangles(VkFormat vertexFormat, VkDeviceSize vertexStride, core::shared_ptr<Acceleration_Structure_Input_Buffer> vertices)
  : Acceleration_Structure_Geometry_Triangles{}
{
  this->geometry.triangles.vertexFormat  = vertexFormat;
  this->geometry.triangles.vertexData    = vk::getAddress(vertices);
  this->geometry.triangles.vertexStride  = vertexStride;
  this->geometry.triangles.maxVertex     = static_cast<core::uint32>(vertices->getSize() / geometry.triangles.vertexStride) - 1;
  this->geometry.triangles.transformData = VkDeviceOrHostAddressConstKHR{};
  this->flags                            = 0;

  primitiveCount = (geometry.triangles.maxVertex + 1) / 3;
}

}
