#version 330 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoords;

uniform sampler2D uTexture;
uniform bool uUseTexture;

uniform vec4 uLightColor;

void main()
{
  if(uUseTexture){
    FragColor = texture(uTexture, vTexCoords) * vColor * uLightColor;
  }else{
    FragColor = vColor * uLightColor;
  }
}
