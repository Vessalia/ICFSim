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
	Model();

	Model(std::vector<Mesh*> meshes);

	~Model();

	void draw(const Material* material) const;

private:

	std::vector<Mesh*> mMeshes;
};
