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
    float val = texture(velocityMap, texCoord).r;
    val += texture(pressureMap, texCoord).r;
    val += texture(inkMap, texCoord).r;
    val += texture(vorocityMap, texCoord).r;
    val *= dt;
	fragColor = vec4(1.0, val, 0.0, 1.0);
}
