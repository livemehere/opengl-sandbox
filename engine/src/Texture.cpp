#include "Texture.hpp"
#include "Engine.hpp"
#include "stb_image.h"
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

Texture::Texture(const char *path) {
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
  if (!data) {
    spdlog::error("Failed to load texture: {}", path);
    throw std::runtime_error("Failed to load texture");
  }

  GLenum format;

  if (channels == 1)
    format = GL_RED;
  else if (channels == 3)
    format = GL_RGB;
  else if (channels == 4)
    format = GL_RGBA;

  GLCALL(glGenTextures(1, &textureID));
  GLCALL(glBindTexture(GL_TEXTURE_2D, textureID));

  GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
  GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
  GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                         GL_LINEAR_MIPMAP_LINEAR));
  GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

  GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                      GL_UNSIGNED_BYTE, data));
  GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
  stbi_image_free(data);

  GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

  spdlog::info("Texture {} loaded and uploaded GPU successfully: {} ({}x{}, {} "
               "channels)",
               textureID, path, width, height, channels);
}

Texture::~Texture() {
  GLCALL(glDeleteTextures(1, &textureID));
  spdlog::info("Texture {} deleted", textureID);
}

void Texture::Bind(unsigned int slot) const {
  GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
  GLCALL(glBindTexture(GL_TEXTURE_2D, textureID));
}

void Texture::UnBind() const { GLCALL(glBindTexture(GL_TEXTURE_2D, 0)); }
