#version 450

layout(row_major) layout(binding = 0) uniform Camera
{
  mat4 worldToClip;
  mat4 worldToView;
  mat4 viewToClip;
  mat4 clipToView;
  mat4 viewToWorld;
  mat4 clipToWorld;
} camera;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = vec4(inPosition, 1.0) * camera.worldToView * camera.viewToClip;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}
