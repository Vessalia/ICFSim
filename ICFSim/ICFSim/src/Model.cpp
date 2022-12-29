#include "Model.h"

Model::Model(std::vector<Mesh*> meshes, Material* material)
	: mMaterial(material)
{
	for (auto* mesh : meshes)
	{
		mMeshes.push_back(mesh);
	}
}

Model::~Model()
{
	for (auto entry : mLoadedTextures)
	{
		delete entry.second;
	}

	for (auto* mesh : mMeshes)
	{
		delete mesh;
	}

	if (mInstanceHandle)
	{
		glDeleteBuffers(1, &mInstanceHandle);
	}
}

void Model::draw() const
{
	mMaterial->use();

	if (mInstanceCount)
	{
		for (size_t i = 0; i < mMeshes.size(); i++)
		{
			mMeshes[i]->drawInstanced(mInstanceCount);
		}
	}
	else
	{
		for (size_t i = 0; i < mMeshes.size(); i++)
		{
			mMeshes[i]->draw();
		}
	}
}

void Model::setMaterial(Material* material)
{
	mMaterial = material;

	for (auto entry : mLoadedTextures)
	{
		mMaterial->pushUniform(entry.first, *entry.second);
	}
}

void Model::setInstanceData(std::vector<glm::mat4> instanceMatrices)
{
	mInstanceCount = instanceMatrices.size();
	glGenBuffers(1, &mInstanceHandle);
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceHandle);
	glBufferData(GL_ARRAY_BUFFER, instanceMatrices.size() * sizeof(glm::mat4), &instanceMatrices[0], GL_STATIC_DRAW);

	for (auto mesh : mMeshes)
	{
		mesh->setInstanceBuffer();
	}
}
