#version 460 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;

out vec2 TexCoord;
out float layerFraction;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int totalLayers;
uniform int layer;
uniform float furLength;

void main(){
  layerFraction = float(layer)/float(totalLayers);
  vec3 displacedPos = aPos + aNormal * furLength * layerFraction;

  gl_Position = projection * view * model * vec4(displacedPos,1.0);
  
  TexCoord = aTexCoord;
  
}
