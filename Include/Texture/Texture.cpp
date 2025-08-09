#include "./Texture.hpp"

Texture::Texture(){
  glGenTextures(1,&id);
}

Texture::~Texture(){
  glDeleteTextures(1,&id);
}

void Texture::CreateTexture(GLenum type){
  this->type = type;
}

void Texture::BindTexture(){
  glBindTexture(type,id);
}

void Texture::UnbindTexture(){
  glBindTexture(type,0);
}

void Texture::SetWrapS(GLenum value){
  glTexParameteri(type,GL_TEXTURE_WRAP_S,value);
}

void Texture::SetWrapT(GLenum value){
  glTexParameteri(type,GL_TEXTURE_WRAP_T,value);
}

void Texture::SetWrapR(GLenum value){
  glTexParameteri(type,GL_TEXTURE_WRAP_R,value);
}

void Texture::SetMinFilter(GLenum value){
  glTexParameteri(type,GL_TEXTURE_MIN_FILTER,value);
}

void Texture::SetMagFilter(GLenum value){
  glTexParameteri(type,GL_TEXTURE_MAG_FILTER,value);
}

void Texture::SetSamplerValue(Shader& shader,const std::string& name, int value){
  shader.Use();
  shader.SetValue(name,value);
}

void Texture::AssignTextureUnit(int value){
  glActiveTexture(GL_TEXTURE0 + value);
}

void Texture::LoadTexture(const std::string& path, bool mipmap){
  stbi_set_flip_vertically_on_load(true);
  
  int width;
  int height;
  int nrChannels;
  
  unsigned char* data = stbi_load(path.c_str(),&width,&height,&nrChannels,0);
  GLenum flag = (nrChannels == 3)? GL_RGB : GL_RGBA;

  if(data){
    glTexImage2D(type,0,flag,width,height,0,flag,GL_UNSIGNED_BYTE,data);
    if(mipmap)
      glGenerateMipmap(type);
  }
  stbi_image_free(data);

  std::cout<<"SUCCESS: Loaded texture: "<<path<<"!"<<std::endl;
}

void Texture::LoadCubemapTexture(const std::vector<std::string>& paths){
  stbi_set_flip_vertically_on_load(false);

  int width;
  int height;
  int nrChannels;

  unsigned char* data;

  for(unsigned int i = 0; i < paths.size(); i++){
    data = stbi_load(paths[i].c_str(),&width,&height,&nrChannels,0);
    GLenum flag = (nrChannels == 3)? GL_RGB : GL_RGBA;
    if(data){
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, flag, width, height, 0, flag, GL_UNSIGNED_BYTE, data);
    }
    else{
      std::cout<<"ERROR: Loading cubemap texture: "<<paths[i]<<std::endl;
    }
    stbi_image_free(data);
  }

  std::cout<<"SUCCESS: Loaded cubemap textures!"<<std::endl;
}
