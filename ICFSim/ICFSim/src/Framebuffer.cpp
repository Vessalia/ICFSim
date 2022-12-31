#include "Framebuffer.h"
#include "iostream"
#include <VertexData.h>

Framebuffer::Framebuffer(unsigned int width, unsigned int height, const std::string& textureName, Material* material, int texComponent, int glAttachment)
    : mFrameTexture(Texture(width, height, texComponent))
    , mTextureName(textureName)
    , mMaterial(material)
    , mWidth(width)
    , mHeight(height)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, GL_TEXTURE_2D, mFrameTexture.ID, 0);
    if (glAttachment == GL_COLOR_ATTACHMENT0)
    {
        glGenRenderbuffers(1, &rID);
        glBindRenderbuffer(GL_RENDERBUFFER, rID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rID);
    }
    else if (glAttachment == GL_DEPTH_ATTACHMENT)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, GL_TEXTURE_2D, mFrameTexture.ID, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    VertexData<float> framePosData;
    VertexData<float> frameTexData;
    framePosData.vertices = quadVertices;
    frameTexData.vertices = quadVertices;

    framePosData.size = 2;
    framePosData.stride = 4;
    framePosData.offset = 0;
    framePosData.attribName = "aPos";
    framePosData.type = AttribType::Position;

    frameTexData.size = 2;
    frameTexData.stride = 4;
    frameTexData.offset = 2;
    frameTexData.attribName = "aTexCoord";
    frameTexData.type = AttribType::TexCoord;

    VertexData<unsigned int> frameIndexData;
    frameIndexData.vertices = quadIndices;

    std::vector<VertexData<float>> frameVertexData;
    frameVertexData.push_back(framePosData);
    frameVertexData.push_back(frameTexData);

    mMaterial->pushUniform(mTextureName, mFrameTexture);
    mFrameModel = new Model({ new Mesh(frameVertexData, frameIndexData, GL_STATIC_DRAW) });
}

Framebuffer::~Framebuffer()
{
    delete mFrameModel;
    delete mMaterial;

    glDeleteRenderbuffers(1, &rID);
    glDeleteFramebuffers(1, &ID);
}

void Framebuffer::resize(unsigned int width, unsigned int height)
{
    mFrameTexture.resize(width, height);
    mMaterial->pushUniform(mTextureName, mFrameTexture);

    glBindRenderbuffer(GL_RENDERBUFFER, rID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::use() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Framebuffer::flush() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    mFrameModel->draw(mMaterial);
}
