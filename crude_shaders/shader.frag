#version 450

layout(binding = 5) uniform sampler2D texSampler;

layout (location = 0) in PerVertexData
{
  vec3 color;
  vec2 texCoord;
};

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = texture(texSampler, texCoord);
  //outColor = vec4(color, 1.0);
}
