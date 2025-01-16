#include <vulkan/vulkan.h>

module crude.gfx.vk.acceleration_structure;

import crude.gfx.vk.vulkan_utils;
import crude.gfx.vk.device;
import crude.gfx.vk.extension;
import crude.gfx.vk.acceleration_structure_storage_buffer;
import crude.gfx.vk.constants;

namespace crude::gfx::vk
{

Acceleration_Structure::Acceleration_Structure(core::shared_ptr<Device>                           device,
                                               VkAccelerationStructureTypeKHR                     structureType,
                                               VkAccelerationStructureCreateFlagsKHR              flags,
                                               VkAccelerationStructureBuildTypeKHR                buildType,
                                               VkBuildAccelerationStructureFlagsKHR               buildFlags,
                                               core::span<const Acceleration_Structure_Geometry>  geometries)
  : m_device{ device }
  , m_type{ structureType }
  , m_buildFlags{ buildFlags }
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
  buildGeometryInfo.flags                    = buildFlags;
  buildGeometryInfo.mode                     = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
  buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
  buildGeometryInfo.dstAccelerationStructure = VK_NULL_HANDLE;
  buildGeometryInfo.geometryCount            = vkGeometry.size();
  buildGeometryInfo.pGeometries              = vkGeometry.data();
  buildGeometryInfo.ppGeometries             = nullptr;
  buildGeometryInfo.scratchData.hostAddress  = nullptr;

  VkAccelerationStructureBuildSizesInfoKHR buildSizesInfo = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR };

  auto vkGetAccelerationStructureBuildSizesKHR = getDeviceExtension<PFN_vkGetAccelerationStructureBuildSizesKHR>(m_device);
  if (vkGetAccelerationStructureBuildSizesKHR)
  {
    vkGetAccelerationStructureBuildSizesKHR(device->getHandle(), buildType, &buildGeometryInfo, maxPrimitiveCounts.data(), &buildSizesInfo);
  }

  m_buffer = core::allocateShared<vk::Acceleration_Structure_Storage_Buffer>(device, buildSizesInfo.accelerationStructureSize);
  
  VkAccelerationStructureCreateInfoKHR accelerationStructureInfo;
  accelerationStructureInfo.sType         = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
  accelerationStructureInfo.pNext         = nullptr;
  accelerationStructureInfo.createFlags   = flags;
  accelerationStructureInfo.buffer        = m_buffer->getHandle();
  accelerationStructureInfo.offset        = 0u;
  accelerationStructureInfo.size          = buildSizesInfo.accelerationStructureSize;
  accelerationStructureInfo.type          = structureType;
  accelerationStructureInfo.deviceAddress = cDeviceAddressNull;

  auto vkCreateAccelerationStructureKHR = getDeviceExtension<PFN_vkCreateAccelerationStructureKHR>(m_device);
  if (vkCreateAccelerationStructureKHR)
  {
    VkResult result = vkCreateAccelerationStructureKHR(device->getHandle(), &accelerationStructureInfo, getPVkAllocationCallbacks(), &m_handle);
    vulkanHandleResult(result, "failed to create acceleration structure");
  }

  m_buildScratchSize = buildSizesInfo.buildScratchSize;
}

core::shared_ptr<Acceleration_Structure_Storage_Buffer> Acceleration_Structure::getStructureStorageBuffer()
{
  return m_buffer;
}

core::uint64 Acceleration_Structure::getReference()
{
  VkAccelerationStructureDeviceAddressInfoKHR info{};
  info.sType                 = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
  info.pNext                 = nullptr;
  info.accelerationStructure = m_handle;
  auto vkGetAccelerationStructureDeviceAddressKHR = getDeviceExtension<PFN_vkGetAccelerationStructureDeviceAddressKHR>(m_device);
  if (vkGetAccelerationStructureDeviceAddressKHR)
  {
    return vkGetAccelerationStructureDeviceAddressKHR(m_device->getHandle(), &info);
  }
  return 0u;
}

VkAccelerationStructureTypeKHR Acceleration_Structure::getType() const
{
  return m_type;
}

VkBuildAccelerationStructureFlagsKHR Acceleration_Structure::getBuildFlags() const
{
  return m_buildFlags;
}

VkDeviceSize Acceleration_Structure::getBuildScratchSize()
{
  return m_buildScratchSize;
}

}
