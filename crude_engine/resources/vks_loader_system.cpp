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
  std::ifstream file(path);
  if (!file.is_open())
  {
    core::logError(core::Debug::Channel::General, "Failed to open the scene file at %s.", path.c_str());
    return;
  }
  //// Load the header
  //fread(&scene->header.marker, sizeof(uint32_t), 1, file);
  //if (scene->header.marker != 0xabcabc) {
  //  printf("The scene file at %s is not a valid *.vks file. Its marker does not match.\n", file_path);
  //  free_scene_loader(&loader, device);
  //  return 1;
  //}
  //fread(&scene->header.version, sizeof(uint32_t), 1, file);
  //if (scene->header.version != 1) {
  //  printf("This renderer only supports *.vks files using version 1 of the file format, but the scene file at %s uses version %u.\n", file_path, scene->header.version);
  //  free_scene_loader(&loader, device);
  //  return 1;
  //}
  //fread(&scene->header.material_count, sizeof(uint64_t), 1, file);
  //fread(&scene->header.triangle_count, sizeof(uint64_t), 1, file);
  //fread(&scene->header.dequantization_factor, sizeof(float), 3, file);
  //fread(&scene->header.dequantization_summand, sizeof(float), 3, file);
}

}