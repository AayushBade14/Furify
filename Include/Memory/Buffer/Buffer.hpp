#pragma once

#include <glad/glad.h>
#include <iostream>
#include <vector>

void UnbindSpecificBuffer(GLenum target){
  glBindBuffer(target,0);
}

void CopyBufferData(GLenum readtarget, GLenum writetarget, size_t readoffset, size_t writeoffset, size_t memsize){
  glCopyBufferSubData(readtarget,writetarget,readoffset,writeoffset,memsize);
}

unsigned int GetUblockIndex(unsigned int id, const std::string& name){
  return glGetUniformBlockIndex(id,name);
}

void BindUblock(unsigned int id, unsigned int index, unsingned int bp){
  glUniformBlockBinding(id,index,bp);
}

class Buffer{
private:
  unsigned int id;
  
  GLenum target;

public:
  Buffer();
  ~Buffer();
  
  const unsigned int& GetId() const {return id;}
  
  void* GetMemoryPointer(GLenum type);
  void UnmapMemoryPointer();

  void CreateBuffer(GLenum target);
  
  void BindBuffer();
  void UnbindBuffer();
  
  void AllocateMemory(size_t memsize, GLenum method);
  void PartialFill(size_t offset, size_t memsize, float* data);
  void PartialFill(size_t offset, size_t memsize, unsigned int* data);

  void AllocateAndFill(size_t memsize, float* data, GLenum method);
  void AllocateAndFill(size_t memsize, unsigned int* data, GLenum method);
  
  void SetBindingPoint(unsigned int bp);
  void SetSpecificBindingPoint(size_t offset, size_t memsize, unsigned int bp);
};
