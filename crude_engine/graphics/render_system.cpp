#include <vulkan/vulkan.hpp>
#include <flecs.h>
#include <directxmath/DirectXMath.h>

module crude.graphics.renderer_system;

import crude.scene.transform;
import crude.scene.mesh;
import crude.graphics.mesh_buffer;
import crude.graphics.pipeline;
import crude.graphics.command_buffer;

namespace crude::graphics
{

struct Per_Mesh
{
  // !TODO Move
  DirectX::XMFLOAT4X4  modelToWorld;
  core::uint32         submeshIndex;
};


void rendereSystemEach(flecs::iter& it, size_t index, core::shared_ptr<scene::Mesh> mesh, core::shared_ptr<graphics::Mesh_Buffer> meshBuffer)
{
  auto renderComponent = it.world().get_mut<Render_Component>();

  renderComponent->submeshesDrawsBufferDescriptor->update(meshBuffer->getSubmeshesDrawsBuffer(), meshBuffer->getSubmeshesDrawsBuffer()->getSize());
  renderComponent->vertexBufferDescriptor->update(meshBuffer->getVerticesBuffer(), meshBuffer->getVerticesBuffer()->getSize());
  renderComponent->meshletBufferDescriptor->update(meshBuffer->getMeshletsBuffer(), meshBuffer->getMeshletsBuffer()->getSize());
  renderComponent->primitiveIndicesBufferDescriptor->update(meshBuffer->getPrimitiveIndicesBuffer(), meshBuffer->getPrimitiveIndicesBuffer()->getSize());
  renderComponent->vertexIndicesBufferDescriptor->update(meshBuffer->getVertexIndicesBuffer(), meshBuffer->getVertexIndicesBuffer()->getSize());

  Per_Mesh perMesh;
  if (it.entity(index).has<scene::Transform>())
  {
    auto transform = it.entity(index).get_ref<scene::Transform>();
    perMesh.modelToWorld = transform->getNodeToWorldFloat4x4();
  }

  for (core::uint32 submeshIndex = 0u; submeshIndex < mesh->submeshes.size(); ++submeshIndex)
  {
    const scene::Sub_Mesh& submesh = mesh->submeshes[submeshIndex];
    renderComponent->textureSamplerDescriptor->update(submesh.texture->getImageView(), submesh.texture->getSampler());

    core::array<VkWriteDescriptorSet, 7u> descriptorWrites;
    renderComponent->perFrameUniformBufferDescriptor->write(descriptorWrites[0]);
    renderComponent->textureSamplerDescriptor->write(descriptorWrites[1]);
    renderComponent->submeshesDrawsBufferDescriptor->write(descriptorWrites[2]);
    renderComponent->vertexBufferDescriptor->write(descriptorWrites[3]);
    renderComponent->meshletBufferDescriptor->write(descriptorWrites[4]);
    renderComponent->primitiveIndicesBufferDescriptor->write(descriptorWrites[5]);
    renderComponent->vertexIndicesBufferDescriptor->write(descriptorWrites[6]);

    renderComponent->commandBuffer->pushDescriptorSet(renderComponent->pipeline, descriptorWrites);
    perMesh.submeshIndex = submeshIndex;
    renderComponent->commandBuffer->pushConstant(renderComponent->pipeline->getPipelineLayout(), perMesh);
    renderComponent->commandBuffer->drawMeshTasks(1u);
  }
}

}