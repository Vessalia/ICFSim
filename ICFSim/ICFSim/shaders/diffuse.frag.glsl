#version 460 core
in vec2 texCoord;

out vec4 fragColor;

uniform float alpha;
uniform float rBeta;
uniform vec2 invDim;
uniform sampler2D x;
uniform sampler2D b;

vec4 get(int u, int v)
{
	vec2 uv = texCoord;
    vec2 step = vec2(u, v) * invDim;
    uv += step;
    uv.x = clamp(uv.x, 0.0, 1.0);
    uv.y = clamp(uv.y, 0.0, 1.0);
    return texture2D(x, uv);
}

vec4 jacobi(sampler2D x, sampler2D b, float alpha, float rBeta)
{
	vec4 xL = get(-1,  0);
	vec4 xR = get( 1,  0);
	vec4 xB = get( 0, -1);
	vec4 xT = get( 0,  1);

	vec4 bC = texture(b, texCoord);
	return (xL + xR + xB + xT + alpha * bC) * rBeta;
}

void main()
{ 
	vec4 colour = jacobi(x, b, alpha, rBeta);
	fragColor = vec4(colour.xyz, 1.0);
}
