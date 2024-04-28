#pragma once
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Material.h"
#include <vector>

class Framebuffer
{
public:
    Framebuffer(unsigned int width, unsigned int height, Material* material, int texComponent = GL_RGBA32F, int glAttachment = GL_COLOR_ATTACHMENT0);

    ~Framebuffer();

    void resize(unsigned int width, unsigned int height);

    void use() const;

    void clear() const;

    void flush() const;

    const Texture& getFrameTexture() const;

private:
    unsigned int ID;
    unsigned int rID;

    unsigned int mWidth;
    unsigned int mHeight;

    int mGlAttachment;

    Texture mFrameTexture;
    Model* mFrameModel;
    Material* mMaterial;

    static const inline std::vector<float> quadVertices =
    {
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    static const inline std::vector<unsigned int> quadIndices =
    {
         0, 1, 2,

         2, 1, 3
    };
};
