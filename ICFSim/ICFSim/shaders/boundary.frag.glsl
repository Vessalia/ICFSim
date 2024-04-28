#version 460 core
in vec2 texCoord;

out vec4 fragColor;

uniform int scale;
uniform vec2 invDim;
uniform sampler2D x;

void main()
{
	vec2 LEFT  = invDim * vec2(-1.0,  0.0);
	vec2 RIGHT = invDim * vec2( 1.0,  0.0);
	vec2 DOWN  = invDim * vec2( 0.0, -1.0);
	vec2 UP    = invDim * vec2( 0.0,  1.0);

	if(texCoord.x <= invDim.x)
	{
		fragColor = scale * texture(x, texCoord + RIGHT);
	}
	else if(texCoord.x >= 1 - invDim.x)
	{
		fragColor = scale * texture(x, texCoord + LEFT);
	}
	else if(texCoord.y >= 1 - invDim.y)
	{
		fragColor = scale * texture(x, texCoord + DOWN);
	}
	else if(texCoord.y <= invDim.y)
	{
		fragColor = scale * texture(x, texCoord + UP);
	}
	else
	{
		fragColor = texture(x, texCoord);
	}
}
