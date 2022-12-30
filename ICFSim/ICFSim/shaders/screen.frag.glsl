#version 460 core
out vec4 fragColor;
  
in vec2 texCoord;

uniform sampler2D screenTexture;

void main()
{ 
    fragColor = texture(screenTexture, texCoord);
}