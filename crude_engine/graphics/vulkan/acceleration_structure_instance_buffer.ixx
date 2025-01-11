module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure_instance_buffer;

export import crude.gfx.vk.buffer;
export import crude.gfx.vk.acceleration_structure_input_buffer;

export namespace crude::gfx::vk
{

template<class T>
class Acceleration_Structure_Instance_Buffer : public Acceleration_Structure_Input_Buffer
{
public:
  explicit Acceleration_Structure_Instance_Buffer(std::shared_ptr<Device> device, core::uint32 instanceCount) noexcept
	: Acceleration_Structure_Input_Buffer(device, instanceCount * sizeof(T)), m_instanceCount{ instanceCount } {}
  core::uint32 getInstanceCount() const { return m_instanceCount; }
private:
  core::uint32 m_instanceCount;
};


}