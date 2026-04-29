#pragma once

class Texture {
private:
  unsigned int textureID;
  int width;
  int height;
  int channels;

public:
  Texture(const char *path);
  ~Texture();

  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;

  void Bind(const unsigned int slot = 0) const;
  void UnBind() const;
};
