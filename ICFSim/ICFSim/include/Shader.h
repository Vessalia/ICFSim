#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Texture.h"

#include <string>
#include <Uniform.h>
#include <map>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	Shader(const char* vertexPath, const char* geometryPath, const char* framentPath);

	~Shader();

	void use() const;

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;

	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;

	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;

	void setMat2(const std::string& name, const glm::mat2& value) const;
	void setMat3(const std::string& name, const glm::mat3& value) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;

	void setTex(const std::string& name, unsigned int texID, GLenum target) const;
	void setCube(const std::string& name, unsigned int cubeID, GLenum target) const;
};
