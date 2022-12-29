#pragma once
#include <variant>
#include <glm/glm.hpp>

enum class UniformType
{
	Bool,
	Int,
	Float,
	Vec2,
	Vec3,
	Vec4,
	Mat2,
	Mat3,
	Mat4,
	Sampler2D,

	INVALID
};

class Uniform
{
public:
	Uniform() : mType(UniformType::INVALID) { }

	template<typename T>
	Uniform(const T& data)
	{
		set(data);
	}

	template<typename T>
	void set(const T& data);

	template<typename T>
	const T& get() const
	{
		return std::get<T>(mData);
	}

	const UniformType getType() const
	{
		return mType;
	}

private:
	using Data = std::variant<bool, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat2, glm::mat3, glm::mat4>;

	UniformType mType;
	Data mData;
};
