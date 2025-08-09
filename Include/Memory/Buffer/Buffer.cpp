#include "./Buffer.hpp"

Buffer::Buffer(){
  glGenBuffers(1,&id);
}

Buffer::~Buffer(){
  glDeleteBuffers(1,&id);
}

void Buffer::CreateBuffer(GLenum target){
  this->target = target;
}

void Buffer::BindBuffer(){
  glBindBuffer(target,id);
}

void Buffer::UnbindBuffer(){
  glBindBuffer(target,0);
}

void Buffer::AllocateMemory(size_t memsize,GLenum method){
  glBufferData(target,memsize,NULL,method);
}

void Buffer::PartialFill(size_t offset, size_t memsize, float* data){
  glBufferSubData(target,offset,memsize,data);
}

void Buffer::PartialFill(size_t offset, size_t memsize, unsigned int* data){
  glBufferSubData(target,offset,memsize,data);
}

void Buffer::AllocateAndFill(size_t memsize, float* data, GLenum method){
  glBufferData(target,memsize,data,method);
}

void Buffer::AllocateAndFill(size_t memsize, unsigned int* data, GLenum method){
  glBufferData(target,memsize,data,method);
}

void* Buffer::GetMemoryPointer(GLenum type){
  return glMapBuffer(target,type);
} 

void Buffer::UnmapMemoryPointer(){
  glUnmapBuffer(target);
}

void Buffer::SetBindingPoint(unsigned int bp){
  glBindBufferBase(target,bp,id);
}

void Buffer::SetSpecificBindingPoint(size_t offset, size_t memsize, unsigned int bp){
  glBindBufferRange(target,bp,id,offset,memsize);
}

