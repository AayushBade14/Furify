#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

enum class ShaderType{
  VERT,
  FRAG,
  GEOM
};

class Shader{
private:
  unsigned int id;

  std::string LoadFile(const std::string& path);
  unsigned int CompileShader(const std::string& srcCode, ShaderType type);
  void CreateShaderProgram(unsigned int& vert, unsigned int& frag);
  void CreateShaderProgram(unsigned int& vert, unsigned int& geom, unsigned int& frag);
public:
  Shader(const std::string& vertPath, const std::string& fragPath);
  Shader(const std::string& vertPath, const std::string& geomPath, const std::string& fragPath);
  ~Shader();
  
  const unsigned int& GetId() const{return id;}
  
  void Use();

  template <typename T>
  void SetValue(const std::string& name, const T& val){
    unsigned int loc = glGetUniformLocation(id,name.c_str());

    if constexpr (std::is_same_v<T,int>)
      glUniform1i(loc,val);
    else if constexpr (std::is_same_v<T,bool>)
      glUniform1i(loc,(int)val);
    else if constexpr (std::is_same_v<T,float>)
      glUniform1f(loc,val);
    else if constexpr (std::is_same_v<T,glm::vec2>)
      glUniform2fv(loc,1,glm::value_ptr(val));
    else if constexpr (std::is_same_v<T,glm::vec3>)
      glUniform3fv(loc,1,glm::value_ptr(val));
    else if constexpr (std::is_same_v<T,glm::mat4>)
      glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(val));
  }
};

