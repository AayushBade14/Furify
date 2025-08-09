#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include "../ThirdParty/STB/stb_image.h"
#include "../Shader/Shader.hpp"

class Texture{
private:
  unsigned int id;
  
  GLenum type;
  

public:
  Texture();
  ~Texture();
  
  const unsigned int& GetId() const {return id;}

  void CreateTexture(GLenum type);

  void BindTexture();
  void UnbindTexture();

  void SetWrapS(GLenum value);
  void SetWrapT(GLenum value);
  void SetWrapR(GLenum value);

  void SetMinFilter(GLenum value);
  void SetMagFilter(GLenum value);

  void SetSamplerValue(Shader& shader,const std::string& name, int value);
  void AssignTextureUnit(int value);
  
  void LoadTexture(const std::string& path, bool mipmap);
  void LoadCubemapTexture(const std::vector<std::string>& paths);
};
