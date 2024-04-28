#version 460 core
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D velocityMap;

void main()
{
    fragColor = vec4(texture(velocityMap, texCoord).rgb, 1.0);
}  