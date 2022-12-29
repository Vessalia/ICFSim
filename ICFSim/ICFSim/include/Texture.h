#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
	unsigned int ID;

	std::string type;
	std::string path;

	const enum State
	{
		Data = 0,
		Storage = 1
	}state;

	Texture(const std::string& path, const std::string& type);
	Texture(unsigned int width, unsigned int height, int glComponent = GL_RGB, int glType = GL_UNSIGNED_BYTE);

	~Texture();

	void use(GLenum target) const;

	void resize(unsigned int width, unsigned int height) const;
};
