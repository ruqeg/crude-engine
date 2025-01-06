#include <functional>
#include <DirectXMath.h>
#include <flecs.h>
#include <vulkan/vulkan.h>
#include <filesystem>
#include <fstream>

module crude.resources.vks_loader_system;

import crude.core.logger;
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
  FILE* file = fopen(path.string().c_str(), "rb");
  if (!file)
  {
    core::logError(core::Debug::Channel::FileIO, "Failed to open the scene file at %s.", path.string().c_str());
    return;
  }

  core::uint32 marker;
  fread(&marker, sizeof(marker), 1, file);
  if (marker != 0xabcabc)
  {
    core::logError(core::Debug::Channel::FileIO, "The scene file at %s is not a valid *.vks file. Its marker does not match.", path.string().c_str());
    fclose(file);
    return;
  }

  core::uint32 version;
  fread(&version, sizeof(version), 1, file);
  if (version != 1)
  {
    core::logError(core::Debug::Channel::FileIO, "Only supports *.vks files using version 1 of the file format, but the scene file at %s uses version %u.", path.string().c_str(), version);
    fclose(file);
    return;
  }

  core::uint64 materialCount, triangleCount;
  core::array<core::float32, 3> dequantizationFactor, dequantizationSummand;
  fread(&materialCount, sizeof(materialCount), 1, file);
  fread(&triangleCount, sizeof(triangleCount), 1, file);
  fread(dequantizationFactor.data(), sizeof(dequantizationFactor[0]), dequantizationFactor.size(), file);
  fread(dequantizationSummand.data(), sizeof(dequantizationSummand[0]), dequantizationSummand.size(), file);

  core::uint32 eofMarker;
  fread(&eofMarker, sizeof(eofMarker), 1, file);
  if (eofMarker != 0xe0fe0f)
  {
    core::logError(core::Debug::Channel::FileIO, "Finished reading data from the scene file at %s but did not encounter an end-of-file marker where expected. Either the file is invalid or the loader is buggy.", path.string().c_str());
    fclose(file);
    return;
  }

  fclose(file);
}

}