target_sources(crude_engine
    PRIVATE
    gbuffer.cpp
    renderer_fullscreen_geometry_system.cpp
    renderer_frame_system.cpp
    renderer_deferred_geometry_system.cpp
    renderer_core_component.cpp
    mesh_buffer.cpp
    texture.cpp
    extension.cpp
    format_helper.cpp
    attachment_description.cpp
    image_descriptor.cpp
    image_2d.cpp
    buffer_descriptor.cpp
    uniform_buffer.cpp
    flush.cpp
    device_queue_descriptor.cpp
    command_buffer.cpp
    buffer.cpp
    command_pool.cpp
    debug_utils_messenger.cpp
    shader_module.cpp
    shader_stage_create_info.cpp
    subpass_dependency.cpp
    subpass_description.cpp
    swap_chain.cpp
    vulkan_allocator.cpp
    instance.cpp
    physical_device.cpp
    pipeline.cpp
    pipeline_layout.cpp
    queue.cpp
    render_pass.cpp
    sampler.cpp
    semaphore.cpp
    fence.cpp
    framebuffer.cpp
    image.cpp
    image_memory_barrier.cpp
    image_subresource_range.cpp
    descriptor_pool.cpp
    descriptor_set.cpp
    descriptor_set_layout.cpp
    device.cpp
    device_memory.cpp
    image_view.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_graphics TYPE CXX_MODULES FILES
    gbuffer.ixx
    renderer_fullscreen_geometry_system.ixx
    renderer_frame_system.ixx
    renderer_deferred_geometry_system.ixx
    renderer_core_component.ixx
    mesh_buffer.ixx
    texture.ixx
    camera_ubo.ixx
    storage_buffer.ixx
    extension.ixx
    push_constant_range.ixx
    scissor.ixx
    viewport.ixx
    surface_capabilities_khr.ixx
    physical_device_properties.ixx
    format_helper.ixx
    format_properties.ixx
    generate_mipmaps.ixx
    image_descriptor.ixx
    sampler_state.ixx
    attachment_load_store_op.ixx
    image_attachment.ixx
    image_2d.ixx
    descriptor_pool_size.ixx
    buffer_descriptor.ixx
    descriptor_set_layout_binding.ixx
    uniform_buffer.ixx
    flush.ixx
    device_queue_descriptor.ixx
    staging_buffer.ixx
    vertex_buffer.ixx
    index_buffer.ixx
    application.ixx
    attachment_description.ixx
    buffer.ixx
    color_blend_state_create_info.ixx
    command_buffer.ixx
    command_pool.ixx
    debug_utils_messenger.ixx
    depth_stencil_state_create_info.ixx
    descriptor_image_info.ixx
    descriptor_pool.ixx
    descriptor_set.ixx
    descriptor_set_layout.ixx
    device.ixx
    device_memory.ixx
    dynamic_state_create_info.ixx
    fence.ixx
    format.ixx
    framebuffer.ixx
    image.ixx
    image_memory_barrier.ixx
    image_subresource_range.ixx
    image_view.ixx
    input_assembly_state_create_info.ixx
    instance.ixx
    multisample_state_create_info.ixx
    physical_device.ixx
    pipeline.ixx
    pipeline_layout.ixx
    queue.ixx
    rasterization_state_create_info.ixx
    render_pass.ixx
    sampler.ixx
    semaphore.ixx
    shader_module.ixx
    shader_stage_create_info.ixx
    subpass_dependency.ixx
    subpass_description.ixx
    surface.ixx
    swap_chain.ixx
    swap_chain_image.ixx
    tessellation_state_create_info.ixx
    vertex_input_state_create_info.ixx
    viewport_state_create_info.ixx
    vulkan_allocator.ixx
    vulkan_object.ixx
    vulkan_utils.ixx)
