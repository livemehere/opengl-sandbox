#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;

out vec4 vColor;
out vec2 vTexCoords;
out vec3 vFragPos;
out vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
  vec4 worldPos = uModel * vec4(aPos, 1.0);
  gl_Position = uProj * uView * worldPos;
  vColor = aColor;
  vTexCoords = aTexCoords;
  vFragPos = vec3(worldPos);
  vNormal = mat3(transpose(inverse(uModel))) * aNormal;
}
