#version 460 core
in vec2 texCoord;

out vec4 fragColor;

uniform float halfrdx;
uniform vec2 invDim;
uniform sampler2D map;

vec4 get(int x, int y)
{
	vec2 uv = texCoord;
    vec2 step = vec2(x, y) * invDim;
    uv += step;
    uv.x = clamp(uv.x, 0.0, 1.0);
    uv.y = clamp(uv.y, 0.0, 1.0);
    return texture2D(map, uv);
}

void main()
{
	vec4 xL = get(-1,  0);
	vec4 xR = get( 1,  0);
	vec4 xB = get( 0, -1);
	vec4 xT = get( 0,  1);

	fragColor = vec4((xR.x - xL.x + xT.y - xB.y) * halfrdx, 0.0, 0.0, 1.0);
}