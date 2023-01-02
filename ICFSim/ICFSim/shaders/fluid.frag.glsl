#version 460 core
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D velocityMap;
uniform sampler2D pressureMap;
uniform sampler2D inkMap;
uniform sampler2D vorocityMap;
uniform float dt;

void main()
{
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
