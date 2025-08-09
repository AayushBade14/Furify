#version 460 core

out vec4 fragColor;

in vec2 TexCoord;
in float layerFraction;

uniform sampler2D mask_tex;
uniform sampler2D fur_tex;

void main(){
  float mask = texture(mask_tex,TexCoord * 10.0).r;
  if(mask < 0.5) discard;

  float alpha = 1.0 - layerFraction;
  vec3 furColor = texture(fur_tex,TexCoord).rgb;
  
  fragColor = vec4(furColor,alpha);
}
