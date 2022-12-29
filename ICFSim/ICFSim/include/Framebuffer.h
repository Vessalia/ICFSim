#pragma once
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Material.h"
#include <vector>

class Framebuffer
{
public:
    Framebuffer(unsigned int width, unsigned int height, const std::string& textureName, Material* material, int texComponent = GL_RGB, int glAttachment = GL_COLOR_ATTACHMENT0);

    ~Framebuffer();

    void resize(unsigned int width, unsigned int height);

    void use() const;

    void flush() const;

private:
    unsigned int ID;
    unsigned int rID;

    unsigned int mWidth;
    unsigned int mHeight;

    Texture mFrameTexture;
    Model* mFrameModel;
    Material* mMaterial;
    std::string mTextureName;

    std::vector<float> quadVertices =
    {
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    std::vector<unsigned int> quadIndices =
    {
         0, 1, 2,

         2, 1, 3
    };
};
