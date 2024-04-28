#version 460 core
in vec2 texCoord;

out vec4 fragColor;

uniform float rdx;
uniform vec2 invDim;
uniform float dt;
uniform float dissipation;
uniform sampler2D x;

vec4 lerp(vec4 a, vec4 b, float t)
{
	return a + t * b;
}

vec4 bilerp(sampler2D map, vec2 pos)
{
	vec4 posT;
	posT.xy = floor(pos - 0.5) + 0.5;
	posT.zw = posT.xy + 1;

	vec2 t = pos - posT.xy;

	vec4 tex11 = texture(x, posT.xy);
	vec4 tex21 = texture(x, posT.zy);
	vec4 tex12 = texture(x, posT.xw);
	vec4 tex22 = texture(x, posT.zw);

	return lerp(lerp(tex11, tex21, t.x), lerp(tex12, tex22, t.x), t.y);
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