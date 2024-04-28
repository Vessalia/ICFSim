#version 460 core
in vec2 texCoord;

out vec4 fragColor;

uniform float halfrdx;
uniform vec2 invDim;
uniform sampler2D mapP;
uniform sampler2D mapW;

vec4 get(int x, int y)
{
	vec2 uv = texCoord;
    vec2 step = vec2(x, y) * invDim;
    uv += step;
    uv.x = clamp(uv.x, 0.0, 1.0);
    uv.y = clamp(uv.y, 0.0, 1.0);
    return texture2D(mapP, uv);
}

void main()
{
	float xL = get(-1,  0).r;
	float xR = get( 1,  0).r;
	float xB = get( 0, -1).r;
	float xT = get( 0,  1).r;

	vec4 colour = texture(mapW, texCoord);
	fragColor = vec4(colour.rg - halfrdx * vec2(xR - xL, xT - xB), colour.ba);
}