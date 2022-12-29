#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "VertexData.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"

class Model
{
public:
	Model(std::vector<Mesh*> meshes, Material* material);

	~Model();

	void draw() const;

	void setMaterial(Material* material);

	void setInstanceData(std::vector<glm::mat4> instanceMatrices);

private:
	unsigned int mInstanceHandle = 0;
	size_t mInstanceCount = 0;

	std::vector<Mesh*> mMeshes;
	Material* mMaterial;

	std::map<std::string, Texture*> mLoadedTextures;
};
