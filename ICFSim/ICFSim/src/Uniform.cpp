#include "Uniform.h"
#include <Texture.h>

#define UNIFORM_SET(T, D)\
template<> void Uniform::set(const D& data)\
{\
	mType = T;\
	mData = data;\
}

UNIFORM_SET(UniformType::Bool, bool);
UNIFORM_SET(UniformType::Int, int);
UNIFORM_SET(UniformType::Float, float);
UNIFORM_SET(UniformType::Vec2, glm::vec2);
UNIFORM_SET(UniformType::Vec3, glm::vec3);
UNIFORM_SET(UniformType::Vec4, glm::vec4);
UNIFORM_SET(UniformType::Mat2, glm::mat2);
UNIFORM_SET(UniformType::Mat3, glm::mat3);
UNIFORM_SET(UniformType::Mat4, glm::mat4);
#undef UNIFORM_SET

template<> void Uniform::set(const Texture& tex)
{
	mType = UniformType::Sampler2D;
	mData = static_cast<int>(tex.ID);
}
