#version 450

layout(binding = 0) uniform UniformBufferObject {
    layout(row_major) mat4 model;
    layout(row_major) mat4 view;
    layout(row_major) mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(inPosition, 1.0) * ubo.view * ubo.proj;
    fragColor = inColor;
}
