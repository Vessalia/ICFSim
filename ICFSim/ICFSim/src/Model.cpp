#include "Model.h"
#include "Constants.h"

Model::Model()
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

	mMeshes.push_back(new Mesh(vertexData, indexData, GL_STATIC_DRAW));
}

Model::Model(std::vector<Mesh*> meshes)
{
	for (auto* mesh : meshes)
	{
		mMeshes.push_back(mesh);
	}
}

Model::~Model()
{
	for (auto* mesh : mMeshes)
	{
		delete mesh;
	}
}

void Model::draw(const Material* material) const
{
	material->use();

	for (size_t i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i]->draw();
	}
}
