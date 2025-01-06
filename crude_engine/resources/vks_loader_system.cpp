#include <functional>
#include <DirectXMath.h>
#include <flecs.h>
#include <vulkan/vulkan.h>
#include <filesystem>
#include <fstream>

module crude.resources.vks_loader_system;

import crude.core.logger;
import crude.gfx.vk.vertex_buffer;
import crude.gfx.vk.staging_buffer;
import crude.gfx.vk.command_buffer;
import crude.gfx.vk.command_pool;

namespace crude::resources
{

flecs::system registerVksLoaderSystem(flecs::world world, core::shared_ptr<VKS_Loader_Context> ctx)
{
  return world.system<VKS_Metadata_Component>("VKSLoaderSystem")
    .ctx(ctx.get())
    .kind(flecs::OnLoad)
    .with<VKS_Loader_Uninitialized_Flag>()
    .run(vksLoaderSystemProcess);
}

// 0 - crude::resources::VKS_Metadata_Component
// 1 - crude::resources::VKS_Loader_Uninitialized_Flag
void vksLoaderSystemProcess(flecs::iter& it)
{
  VKS_Loader_Context* ctx = it.ctx<VKS_Loader_Context>();
  while (it.next())
  {
    auto metadata = it.field<crude::resources::VKS_Metadata_Component>(0);

    for (auto i : it)
    {
      flecs::entity entity = it.entity(i);

      VKS_Loader gltfLoader(ctx->transferCommandPool);
      gltfLoader.loadToNodeFromFile(entity, metadata[i].path);
      ctx->callback(entity);
      entity.remove<crude::resources::VKS_Loader_Uninitialized_Flag>();
    }
  }
}

VKS_Loader::VKS_Loader(core::shared_ptr<gfx::vk::Command_Pool> commandPool)
  : m_commandPool{ commandPool }
{}

void VKS_Loader::loadToNodeFromFile(flecs::entity node, const std::filesystem::path& path)
{
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open())
  {
    core::logError(core::Debug::Channel::FileIO, "Failed to open the scene file at %s.", path.string().c_str());
    return;
  }

  core::uint32 marker;
  file.read(reinterpret_cast<char*>(&marker), sizeof(marker));
  if (marker != 0xabcabc)
  {
    core::logError(core::Debug::Channel::FileIO, "The scene file at %s is not a valid *.vks file. Its marker does not match.", path.string().c_str());
    return;
  }

  core::uint32 version;
  file.read(reinterpret_cast<char*>(&version), sizeof(version));
  if (version != 1)
  {
    core::logError(core::Debug::Channel::FileIO, "Only supports *.vks files using version 1 of the file format, but the scene file at %s uses version %u.", path.string().c_str(), version);
    return;
  }

  core::uint64 materialCount, triangleCount;
  core::array<core::float32, 3> dequantizationFactor, dequantizationSummand;
  file.read(reinterpret_cast<char*>(&materialCount), sizeof(materialCount));
  file.read(reinterpret_cast<char*>(&triangleCount), sizeof(triangleCount));
  file.read(reinterpret_cast<char*>(dequantizationFactor.data()), sizeof(dequantizationFactor[0]) * dequantizationFactor.size());
  file.read(reinterpret_cast<char*>(dequantizationSummand.data()), sizeof(dequantizationSummand[0]) * dequantizationSummand.size());

  core::vector<core::string> materialNames(materialCount);
  for (core::uint64 i = 0; i != materialCount; ++i)
  {
    core::uint64 length;
    file.read(reinterpret_cast<char*>(&length), sizeof(length));

    materialNames[i].resize(length + 1);
    file.read(&materialNames[i][0], materialNames[i].size());
  }

  // !TODO T_T
  core::vector<char> data;
  data.resize(sizeof(core::uint32) * 2u * triangleCount * 3u);
  file.read(data.data(), data.size());
  auto commandBuffer = core::allocateShared<gfx::vk::Command_Buffer>(m_commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  core::shared_ptr<gfx::vk::Vertex_Buffer> positionBuffer = core::allocateShared<gfx::vk::Vertex_Buffer>(commandBuffer, data);

  data.resize(sizeof(core::uint32) * 2u * triangleCount * 3u);
  file.read(data.data(), data.size());
  core::shared_ptr<gfx::vk::Vertex_Buffer> normalTexcoordBuffer = core::allocateShared<gfx::vk::Vertex_Buffer>(commandBuffer, data);

  data.resize(sizeof(core::uint8) * triangleCount);
  file.read(data.data(), data.size());
  core::shared_ptr<gfx::vk::Vertex_Buffer> materialIndicesBuffer = core::allocateShared<gfx::vk::Vertex_Buffer>(commandBuffer, data);

  core::uint32 eofMarker;
  file.read(reinterpret_cast<char*>(&eofMarker), sizeof(eofMarker));
  if (eofMarker != 0xe0fe0f)
  {
    core::logError(core::Debug::Channel::FileIO, "Finished reading data from the scene file at %s but did not encounter an end-of-file marker where expected. Either the file is invalid or the loader is buggy.", path.string().c_str());
    return;
  }

  // !TODO
}

}