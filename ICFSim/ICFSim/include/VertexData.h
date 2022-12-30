#pragma once

#include <vector>
#include <string>

enum class AttribType
{
	Position = 0,
	TexCoord = 1,
	Normal = 2,
	Instance = 3,

	NEXT = 7
};

const std::vector<float> QUAD_VERTICES =
{
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	-1.0f,  1.0f,
	 1.0f,  1.0f
};

const std::vector<float> QUAD_TEX_COORDS =
{
	 0.0f,  0.0f,
	 1.0f,  0.0f,
	 0.0f,  1.0f,
	 1.0f,  1.0f
};

const std::vector<unsigned int> QUAD_INDICES =
{
	0, 1, 2,
	2, 1, 3
};

template <typename T>
struct VertexData
{
	std::vector<T> vertices;

	unsigned int size = 0;
	unsigned int stride = 0;
	unsigned int offset = 0;

	std::string attribName;

	AttribType type = AttribType::NEXT;
};
