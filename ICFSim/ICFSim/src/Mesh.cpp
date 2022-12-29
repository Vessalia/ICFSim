#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<VertexData<float>>& vertexData,
    const VertexData<unsigned int>& indexData, unsigned int renderHint)
    : vertexCount(vertexData.size())
    , indices(indexData)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    for (int i = 0; i < vertexData.size(); i++)
    {
        VBOs.push_back(0);
    }
    glGenBuffers((GLsizei)vertexData.size(), &VBOs[0]);
    for (int i = 0; i < vertexData.size() && i < 16; i++)
    {
        int type = static_cast<int>(vertexData[i].type);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, vertexData[i].vertices.size() * sizeof(float), &vertexData[i].vertices[0], renderHint);

        glVertexAttribPointer(type, vertexData[i].size, GL_FLOAT, GL_FALSE, vertexData[i].stride * sizeof(float), (void*)(vertexData[i].offset * sizeof(float)));
        glEnableVertexAttribArray(type);
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.vertices.size() * sizeof(unsigned int), &indexData.vertices[0], renderHint);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers((GLsizei)vertexCount, &VBOs[0]);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.vertices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::drawInstanced(size_t instanceCount) const
{
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indices.vertices.size(), GL_UNSIGNED_INT, 0, (GLsizei)instanceCount);
}

void Mesh::setInstanceBuffer()
{
    glBindVertexArray(VAO);
    int attribLocation = static_cast<int>(AttribType::Instance);
    for (unsigned int i = 0; i < 4; ++i)
    {
        glEnableVertexAttribArray(attribLocation + i);
        glVertexAttribPointer(attribLocation + i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(attribLocation + i, 1);
    }
    glBindVertexArray(0);
}
