#version 460 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D wall_tex;

void main(){
  fragColor = texture(wall_tex,TexCoord);
}
