#version 460 core
in vec2 texCoord;

out vec4 fragColor;

uniform float invR;
uniform vec2 impulsePos;
uniform vec4 Fdt; // force for velocity, colour for ink
uniform sampler2D base;

vec4 calcForce(vec2 pos, float invRad)
{
	return Fdt.xyzz * exp(-dot(pos, pos) * invRad);
}

void main()
{
	vec2 screenSize = textureSize(base, 0).xy;
	vec2 pos = screenSize * (impulsePos - texCoord);
	vec4 color = calcForce(pos, invR / screenSize.x);

	fragColor = texture(base, texCoord) + color;
}