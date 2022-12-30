#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Constants.h"

Model::Model(float width, float height, Material* material)
	: mMaterial(material)
{
	VertexData<float> positionData;
	positionData.vertices = QUAD_VERTICES;
	positionData.size = 2;
	positionData.stride = 2;
	positionData.offset = 0;
	positionData.attribName = "aPos";
	positionData.type = AttribType::Position;

	VertexData<float> texCoordData;
	texCoordData.vertices = QUAD_TEX_COORDS;
	texCoordData.size = 2;
	texCoordData.stride = 2;
	texCoordData.offset = 0;
	texCoordData.attribName = "aTexCoord";
	texCoordData.type = AttribType::TexCoord;

	std::vector<VertexData<float>> vertexData;
	vertexData.push_back(positionData);
	vertexData.push_back(texCoordData);

	VertexData<unsigned int> indexData;
	indexData.vertices = QUAD_INDICES;

	glm::mat4 model(1.0f);
	mMaterial->pushUniform("model", glm::scale(model, glm::vec3(width / SCREEN_WIDTH, height / SCREEN_HEIGHT, 1.0f)));

	mMeshes.push_back(new Mesh(vertexData, indexData, GL_STATIC_DRAW));
}

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
