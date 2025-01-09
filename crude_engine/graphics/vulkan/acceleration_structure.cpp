#include <vulkan/vulkan.h>

module crude.gfx.vk.acceleration_structure;

import crude.gfx.vk.device;

namespace crude::gfx::vk
{

Acceleration_Structure::Acceleration_Structure(core::shared_ptr<Device>                                     device,
                                               VkAccelerationStructureTypeKHR                               structureType,
                                               VkAccelerationStructureCreateFlagsKHR                        flags,
                                               VkAccelerationStructureBuildTypeKHR                          buildType,
                                               VkBuildAccelerationStructureFlagsKHR                         buildFlags,
                                               core::span<const Acceleration_Structure_Geometry_Triangles>  geometries)
  : m_device{ device }
{
  core::vector<VkAccelerationStructureGeometryKHR> vkGeometry(geometries.begin(), geometries.end());
  core::vector<core::uint32> maxPrimitiveCounts(geometries.size());
  for (core::uint32 i = 0; i < maxPrimitiveCounts.size(); ++i)
  {
    maxPrimitiveCounts[i] = geometries[i].primitiveCount;
  }

  VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo{};
  buildGeometryInfo.sType                    = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
  buildGeometryInfo.pNext                    = nullptr;
  buildGeometryInfo.type                     = structureType;
  buildGeometryInfo.flags                    = 0;
  buildGeometryInfo.mode                     = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
  buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
  buildGeometryInfo.dstAccelerationStructure = VK_NULL_HANDLE;
  buildGeometryInfo.geometryCount            = vkGeometry.size();
  buildGeometryInfo.pGeometries              = vkGeometry.data();
  buildGeometryInfo.ppGeometries             = nullptr;
  buildGeometryInfo.scratchData.hostAddress  = nullptr;

  VkAccelerationStructureBuildSizesInfoKHR buildSizesInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR };
  //vkGetAccelerationStructureBuildSizesKHR(device->getHandle(), buildType, &buildGeometryInfo, maxPrimitiveCounts.data(), &buildSizesInfo);
  
  //buffer = std::make_unique<AccelerationStructureStorageBuffer>(std::move(device),
  //  buildSizesInfo.accelerationStructureSize, buildType, std::move(allocator), Buffer::Initializer(), sharing);

  //VkAccelerationStructureCreateInfoKHR createInfo{};
  //createInfo.sType          = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
  //createInfo.pNext          = nullptr;
  //createInfo.createFlags    = 0;
  //createInfo.buffer         = ;
  //createInfo.offset         =;
  //createInfo.size           =;
  //createInfo.type           =;
  //createInfo.deviceAddress  =;
}

}
