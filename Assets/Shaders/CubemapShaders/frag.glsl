#version 460 core

out vec4 fragColor;

in vec3 TexDir;

uniform samplerCube sky;

void main(){
  fragColor = texture(sky,TexDir);
}
