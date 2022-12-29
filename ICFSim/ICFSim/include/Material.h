#pragma once
#include "Uniform.h"
#include "Shader.h"
#include <map>

class Material
{
public:
	Material(Shader* shader);

	~Material();

	void pushUniform(const std::string& name, const Uniform& uniform);

	void use() const;

	Shader* getShader() const
	{
		return mShader;
	}

private:
	Shader* mShader;
	std::map<std::string, Uniform> mUniforms;
};
