#pragma once

#include <glad/glad.h>
#include <iostream>
#include "../Buffer/Buffer.hpp"

class VAO{
private:
  unsigned int id;

public:
  VAO();
  ~VAO();
  
  const unsigned int& GetId() const {return id;}

  void Bind();
  void Unbind();
  
  void AssociateBuffer(Buffer& buffer);

  void SetAttribPointer(int loc, int nr, int stride, int start);
  void SetAttribPointer(int loc, int nr, GLenum type, GLBoolean isnormalized, int stride, int start);

  void SetAttribIPointer(int loc, int nr, int stride, int start);
};
