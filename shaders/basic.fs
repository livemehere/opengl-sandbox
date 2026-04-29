#version 330 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoords;
in vec3 vFragPos;
in vec3 vNormal;

uniform sampler2D uTexture;
uniform bool uUseTexture;
uniform bool uUnlit;

struct Light {
  vec3 position;
  vec3 color;
  float ambientStrength;
  float diffuseStrength;
  float specularStrength;
  float shininess;
};

uniform Light uLight;
uniform vec3 uViewPos;

void main()
{
  vec4 baseColor = uUseTexture ? texture(uTexture, vTexCoords) * vColor : vColor;

  if (uUnlit) {
    FragColor = baseColor;
    return;
  }

  vec3 norm = normalize(vNormal);
  vec3 lightDir = normalize(uLight.position - vFragPos);
  vec3 viewDir = normalize(uViewPos - vFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float diff = max(dot(norm, lightDir), 0.0);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), uLight.shininess);

  vec3 ambient = uLight.ambientStrength * uLight.color;
  vec3 diffuse = uLight.diffuseStrength * diff * uLight.color;
  vec3 specular = uLight.specularStrength * spec * uLight.color;

  vec3 litColor = baseColor.rgb * (ambient + diffuse) + specular;
  FragColor = vec4(litColor, baseColor.a);
}
