#version 460 core
in vec2 texCoord;

out vec4 fragColor;

uniform float rdx;
uniform vec2 invDim;
uniform float dt;
uniform float dissipation;
uniform sampler2D x;

vec4 bilerp(sampler2D map, vec2 pos)
{
	vec2 relPosTL = vec2(int(pos.x), int(pos.y));
	vec2 relPosBR = relPosTL + vec2(1, -1);
	float pL = relPosTL.x;
	float pR = relPosBR.x;
	float pB = relPosBR.y;
	float pT = relPosTL.y;

	float tx = (pos.x - pL) / (pR - pL);
	float ty = (pos.y - pT) / (pB - pT);

	vec2 pos1X = vec2(pL, pT) + vec2(pR, pT) * tx;
	vec2 pos2X = vec2(pL, pB) + vec2(pR, pB) * tx;

	vec2 point = pos1X + pos2X * ty;

	point *= invDim;

	return texture(map, point);
}

vec4 advect(sampler2D map)
{
	vec2 pos = textureSize(x, 0).xy * (texCoord - dt * rdx * texture(map, texCoord).xy); 
	return dissipation * bilerp(map, pos);
}

void main()
{ 
	fragColor = vec4(advect(x).rgb, 1.0);
}