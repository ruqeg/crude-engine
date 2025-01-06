module;

#include <functional>
#include <DirectXMath.h>
#include <flecs.h>
#include <vulkan/vulkan.h>
#include <filesystem>

export module crude.resources.vks_loader_system;

export import crude.scene.mesh;
export import crude.scene.light;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

export namespace crude::gfx::vk
{

class Device;
class Command_Pool;

}

export namespace crude::resources
{

using VKS_Loader_Callback_Function = std::function<void(flecs::entity)>;

struct VKS_Loader_Uninitialized_Flag {};

struct VKS_Metadata_Component
{
  std::filesystem::path path;
};

struct VKS_Loader_Context
{
  core::shared_ptr<gfx::vk::Command_Pool>  transferCommandPool;
  VKS_Loader_Callback_Function             callback = [](flecs::entity) {};
};

flecs::system registerVksLoaderSystem(flecs::world world, core::shared_ptr<VKS_Loader_Context> ctx);

}

namespace crude::resources
{

class VKS_Loader
{
public:
  VKS_Loader(core::shared_ptr<gfx::vk::Command_Pool> commandPool);
public:
  void loadToNodeFromFile(flecs::entity node, const std::filesystem::path& path);
private:
  core::shared_ptr<gfx::vk::Command_Pool> m_commandPool;
};

// 0 - crude::resources::VKS_Metadata_Component
// 1 - crude::resources::VKS_Loader_Uninitialized_Flag
void vksLoaderSystemProcess(flecs::iter& it);

}