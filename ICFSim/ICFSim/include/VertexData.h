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
