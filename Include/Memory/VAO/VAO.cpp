#include "./VAO.hpp"

VAO::VAO(){
  glGenVertexArrays(1,&id);
}

VAO::~VAO(){
  glDeleteVertexArrays(1,&id);
}

void VAO::Bind(){
  glBindVertexArray(id);
}

void VAO::Unbind(){
  glBindVertexArray(0);
}

void VAO::SetAttribPointer(int loc, int nr, int stride, int start){
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc,nr,GL_FLOAT,GL_FALSE,stride*sizeof(float),(void*)(start*sizeof(float)));
}

void VAO::SetAttribPointer(int loc, int nr, GLenum type, GLBoolean isnormalized, size_t stride, size_t start){
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc,nr,type,isnormalized,stride,(void*)start);
}

void VAO::SetAttribIPointer(int loc, int nr, int stride, int start){
  glEnableVertexAttribArray(loc);
  glVertexAttribIPointer(loc,nr,GL_INT,stride*sizeof(int),(void*)(start*sizeof(int)));
}

