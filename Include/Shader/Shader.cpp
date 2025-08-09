#include "./Shader.hpp"

Shader::Shader(const std::string& vertPath, const std::string& fragPath){
  std::string vertCode = LoadFile(vertPath);
  std::string fragCode = LoadFile(fragPath);

  unsigned int vert = CompileShader(vertCode,ShaderType::VERT);
  unsigned int frag = CompileShader(fragCode,ShaderType::FRAG);

  CreateShaderProgram(vert,frag);
}

Shader::Shader(const std::string& vertPath, const std::string& geomPath, const std::string& fragPath){
  std::string vertCode = LoadFile(vertPath);
  std::string geomCode = LoadFile(geomPath);
  std::string fragCode = LoadFile(fragPath);

  unsigned int vert = CompileShader(vertCode,ShaderType::VERT);
  unsigned int geom = CompileShader(geomCode,ShaderType::GEOM);
  unsigned int frag = CompileShader(fragCode,ShaderType::FRAG);

  CreateShaderProgram(vert,geom,frag);
}

Shader::~Shader(){
  glDeleteProgram(id);
}

std::string Shader::LoadFile(const std::string& path){
  std::string code;
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try{
    file.open(path);
    
    std::stringstream stream;
    stream << file.rdbuf();
    
    file.close();
    code = stream.str();
  }
  catch(const std::ifstream::failure& e){
    std::cerr<<"ERROR: Reading File: "<<path<<" | REASON: "<<e.what()<<std::endl;
  }

  return code;
}

unsigned int Shader::CompileShader(const std::string& srcCode, ShaderType type){
  const char* code = srcCode.c_str();

  int success;
  char infoLog[512];
  unsigned int shader = (type == ShaderType::VERT) ? glCreateShader(GL_VERTEX_SHADER) : ((type == ShaderType::FRAG) ? glCreateShader(GL_FRAGMENT_SHADER) : glCreateShader(GL_GEOMETRY_SHADER));
  
  std::string sType =  (type == ShaderType::VERT) ? "VERTEX_SHADER" : ((type == ShaderType::FRAG) ? "FRAGMENT_SHADER" : "GEOMETRY_SHADER");
 
  glShaderSource(shader,1,&code,NULL);
  glCompileShader(shader);

  glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
  if(!success){
    glGetShaderInfoLog(shader,512,NULL,infoLog);
    std::cerr<<"ERROR: Compiling "<<sType<<" | REASON: "<<infoLog<<std::endl;
  }
  else{
    std::cout<<"SUCCESS: Created "<<sType<<"!"<<std::endl;
  }

  return shader;
}

void Shader::CreateShaderProgram(unsigned int& vert, unsigned int& frag){
  int success;
  char infoLog[512];

  id = glCreateProgram();
  glAttachShader(id,vert);
  glAttachShader(id,frag);
  glLinkProgram(id);

  glGetProgramiv(id,GL_LINK_STATUS,&success);
  if(!success){
    glGetProgramInfoLog(id,512,NULL,infoLog);
    std::cerr<<"ERROR: Creating Shader Program!"<<std::endl;
  }
  else{
    std::cout<<"SUCCESS: Creating Shader Program!"<<std::endl;
  }

  glDeleteShader(vert);
  glDeleteShader(frag);
}

void Shader::CreateShaderProgram(unsigned int& vert, unsigned int& geom, unsigned int& frag){
  int success;
  char infoLog[512];

  id = glCreateProgram();
  glAttachShader(id,vert);
  glAttachShader(id,geom);
  glAttachShader(id,frag);
  glLinkProgram(id);

  glGetProgramiv(id,GL_LINK_STATUS,&success);
  if(!success){
    glGetProgramInfoLog(id,512,NULL,infoLog);
    std::cerr<<"ERROR: Creating Shader Program!"<<std::endl;
  }
  else{
    std::cout<<"SUCCESS: Creating Shader Program!"<<std::endl;
  }

  glDeleteShader(vert);
  glDeleteShader(geom);
  glDeleteShader(frag);
}

void Shader::Use(){
  glUseProgram(id);
}
