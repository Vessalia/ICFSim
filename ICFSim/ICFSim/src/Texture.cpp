#include "Texture.h"
#include <SDL.h>
#include <SDL_image.h>

Texture::Texture(const std::string& path, const std::string& type)
	: path(path)
	, type(type)
	, state(Data)
{
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	glGenTextures(1, &ID);
	use(GL_TEXTURE0);

	int mode = GL_RGB;
	if (loadedSurface->format->BytesPerPixel == 4)
	{
		mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, mode, loadedSurface->w, loadedSurface->h, 0, mode, GL_UNSIGNED_BYTE, loadedSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(loadedSurface);
}

Texture::Texture(unsigned int width, unsigned int height, int glComponent, int glType)
	: state(Storage)
{
	glGenTextures(1, &ID);
	use(GL_TEXTURE0);

	glTexImage2D(GL_TEXTURE_2D, 0, glComponent, width, height, 0, glComponent, glType, NULL);

	if (glComponent == GL_DEPTH_COMPONENT)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::resize(unsigned int width, unsigned int height) const
{
	use(GL_TEXTURE0);

	if (state == Storage)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::use(GLenum target) const
{
	glActiveTexture(target);
	glBindTexture(GL_TEXTURE_2D, ID);
}
