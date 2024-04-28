#include "Framebuffer.h"
#include "iostream"
#include <VertexData.h>
#include "Constants.h"

Framebuffer::Framebuffer(unsigned int width, unsigned int height, Material* material, int texComponent, int glAttachment)
    : mFrameTexture(Texture(width, height, texComponent, GL_FLOAT))
    , mGlAttachment(glAttachment)
    , mMaterial(material)
    , mWidth(width)
    , mHeight(height)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    if (glAttachment == GL_DEPTH_ATTACHMENT)
    {
        glGenRenderbuffers(1, &rID);
        glBindRenderbuffer(GL_RENDERBUFFER, rID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rID);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    else if (glAttachment == GL_COLOR_ATTACHMENT0)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, GL_TEXTURE_2D, mFrameTexture.ID, 0);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    VertexData<float> framePosData;
    VertexData<float> frameTexData;
    framePosData.vertices = Framebuffer::quadVertices;
    frameTexData.vertices = Framebuffer::quadVertices;

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

    mFrameModel = new Model({ new Mesh(frameVertexData, frameIndexData, GL_STATIC_DRAW) });
    clear();
}

Framebuffer::~Framebuffer()
{
    delete mFrameModel;

    glDeleteRenderbuffers(1, &rID);
    glDeleteFramebuffers(1, &ID);
}

void Framebuffer::resize(unsigned int width, unsigned int height)
{
    mWidth = width; mHeight = height;
    mFrameTexture.resize(width, height);
    mMaterial->pushUniform("screenTexture", mFrameTexture);

    glBindRenderbuffer(GL_RENDERBUFFER, rID);
    if (mGlAttachment == GL_COLOR_ATTACHMENT0) 
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, mGlAttachment, GL_TEXTURE_2D, mFrameTexture.ID, 0);
    }
    else if (mGlAttachment == GL_DEPTH_ATTACHMENT) 
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rID);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::use() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glViewport(0, 0, mWidth, mHeight);
}

void Framebuffer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::flush() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    mMaterial->pushUniform("screenTexture", mFrameTexture);
    mFrameModel->draw(mMaterial);
}

const Texture& Framebuffer::getFrameTexture() const
{
    return mFrameTexture;
}
