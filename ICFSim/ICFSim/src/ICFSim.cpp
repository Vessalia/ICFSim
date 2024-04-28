#include <iostream>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Constants.h"
#include "Framebuffer.h"

SDL_GLContext gContext;

SDL_Window* gWindow = nullptr;

Material* screenMaterial;
Material* fluidMaterial;

Framebuffer* framebuffer = nullptr;

Uint32 lastUpdateTime = SDL_GetTicks();
float deltaTime; // need global to be used in the SDL event system

Shader* advectShader;
Material* advectMaterial;

Shader* diffuseShader;
Material* diffuseMaterial;

Shader* impulseShader;
Material* impulseMaterial;

Shader* divShader;
Material* divMaterial;

Shader* gradShader;
Material* gradMaterial;

Shader* boundaryShader;
Material* boundaryMaterial;

Model* screen;
Model* fluid;

Framebuffer* velReadFramebuffer;
Framebuffer* psiReadFramebuffer;
Framebuffer* inkReadFramebuffer;
Framebuffer* vorReadFramebuffer;

Framebuffer* velWriteFramebuffer;
Framebuffer* psiWriteFramebuffer;
Framebuffer* inkWriteFramebuffer;
Framebuffer* vorWriteFramebuffer;

Framebuffer* tempReadFramebuffer;
Framebuffer* tempWriteFramebuffer;

bool init();
void draw(float deltaTime);
void close();
void handleMovement(SDL_Event* event, float dt);

int main()
{
    if (!init())
    {
        std::cout << "Failed to initialize!" << std::endl;
        return 1;
    }

    bool quit = false;
    bool turning = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    glViewport(0, 0, e.window.data1, e.window.data2);
                    if (framebuffer)
                    {
                        framebuffer->resize(e.window.data1, e.window.data2);
                    }
                }
            }
            
            if (e.type == SDL_MOUSEMOTION)
            {
                handleMovement(&e, deltaTime);
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        deltaTime = (SDL_GetTicks() - lastUpdateTime) / 1000.f;
        draw(1 / 60.0);
        lastUpdateTime = SDL_GetTicks();

        SDL_GL_SwapWindow(gWindow);
    }

    close();

    return 0;
}

static void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
    auto const src_str = [source]()
    {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        }
        return "";
    }();

    auto const type_str = [type]()
    {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        }
        return "";
    }();

    auto const severity_str = [severity]()
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        }
        return "";
    }();
    std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize. SDL Error: " << SDL_GetError() << "\n" << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    gWindow = SDL_CreateWindow("LearnOpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    gContext = SDL_GL_CreateContext(gWindow);

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL.\n" << std::endl;
        return false;
    }

    glDepthFunc(GL_LESS);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLErrorCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    screenMaterial = new Material(new Shader("shaders/screen.vert.glsl", "shaders/screen.frag.glsl"));
    fluidMaterial = new Material(new Shader("shaders/screen.vert.glsl", "shaders/fluid.frag.glsl"));

    framebuffer = new Framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, screenMaterial);

    velWriteFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);
    psiWriteFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);
    inkWriteFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);
    vorWriteFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);

    velReadFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);
    psiReadFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);
    inkReadFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);
    vorReadFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);

    tempReadFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);
    tempWriteFramebuffer = new Framebuffer(TEX_RES_X, TEX_RES_Y, screenMaterial);

    advectShader = new Shader("shaders/screen.vert.glsl", "shaders/advect.frag.glsl");
    advectMaterial = new Material(advectShader);
    advectMaterial->pushUniform("invDim", INV_DIM);
    advectMaterial->pushUniform("rdx", 1 / DX);

    diffuseShader = new Shader("shaders/screen.vert.glsl", "shaders/diffuse.frag.glsl");
    diffuseMaterial = new Material(diffuseShader);
    diffuseMaterial->pushUniform("invDim", INV_DIM);

    impulseShader = new Shader("shaders/screen.vert.glsl", "shaders/impulse.frag.glsl");
    impulseMaterial = new Material(impulseShader);
    impulseMaterial->pushUniform("invR", 1 / BRUSH_RADIUS);

    divShader = new Shader("shaders/screen.vert.glsl", "shaders/div.frag.glsl");
    divMaterial = new Material(divShader);
    divMaterial->pushUniform("halfrdx", 0.5f / DX);
    divMaterial->pushUniform("invDim", INV_DIM);

    gradShader = new Shader("shaders/screen.vert.glsl", "shaders/grad.frag.glsl");
    gradMaterial = new Material(gradShader);
    gradMaterial->pushUniform("halfrdx", 0.5f / DX);
    gradMaterial->pushUniform("invDim", INV_DIM);

    boundaryShader = new Shader("shaders/screen.vert.glsl", "shaders/boundary.frag.glsl");
    boundaryMaterial = new Material(boundaryShader);
    boundaryMaterial->pushUniform("invDim", INV_DIM);

    screen = new Model();

    return true;
}

bool doImpulse = false;
int prevMouseX = 0;
int prevMouseY = 0;
void handleMovement(SDL_Event* event, float dt)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (event->type == SDL_MOUSEMOTION && event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        float dx = x - prevMouseX;
        float dy = y - prevMouseY; 
        float valX = (float)x / SCREEN_WIDTH;
        float valY = 1 - (float)y / SCREEN_HEIGHT;

        glm::vec2 pos(valX, valY);
        float forceX = std::min(std::max(dx / SCREEN_WIDTH, -1.f), 1.f);
        float forceY = std::min(std::max(dy / SCREEN_HEIGHT, -1.f), 1.f);
        glm::vec4 force(forceX, forceY, 0, 0);
        impulseMaterial->pushUniform("impulsePos", pos);
        impulseMaterial->pushUniform("Fdt", force);

        prevMouseX = x;
        prevMouseY = y;
        doImpulse = true;
    }
}

void swapBuffers()
{
    std::swap(velReadFramebuffer, velWriteFramebuffer);
    std::swap(psiReadFramebuffer, psiWriteFramebuffer);
    std::swap(inkReadFramebuffer, inkWriteFramebuffer);
    std::swap(vorReadFramebuffer, vorWriteFramebuffer);

    std::swap(tempReadFramebuffer, tempWriteFramebuffer);
}

void draw(float deltaTime)
{
    // Impulse
    if (doImpulse)
    {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "impulse step");
        velWriteFramebuffer->use();
        impulseMaterial->pushUniform("base", velReadFramebuffer->getFrameTexture());
        screen->draw(impulseMaterial);
        std::swap(velWriteFramebuffer, velReadFramebuffer);

        inkWriteFramebuffer->use();
        impulseMaterial->pushUniform("base", inkReadFramebuffer->getFrameTexture());
        impulseMaterial->pushUniform("Fdt", INK_COLOUR);
        screen->draw(impulseMaterial);
        std::swap(inkWriteFramebuffer, inkReadFramebuffer);
        glPopDebugGroup();

        doImpulse = false;
    }

    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "advection step");
    // Advect
    velWriteFramebuffer->use();
    boundaryMaterial->pushUniform("scale", -1);
    boundaryMaterial->pushUniform("x", velReadFramebuffer->getFrameTexture());
    screen->draw(boundaryMaterial);
    std::swap(velWriteFramebuffer, velReadFramebuffer);

    velWriteFramebuffer->use();
    advectMaterial->pushUniform("x", velReadFramebuffer->getFrameTexture());
    advectMaterial->pushUniform("dissipation", VELOCITY_DISSIPATION);
    advectMaterial->pushUniform("dt", deltaTime);
    screen->draw(advectMaterial);
    std::swap(velWriteFramebuffer, velReadFramebuffer);

    inkWriteFramebuffer->use();
    boundaryMaterial->pushUniform("scale", 0);
    boundaryMaterial->pushUniform("x", inkReadFramebuffer->getFrameTexture());
    screen->draw(boundaryMaterial);
    std::swap(inkWriteFramebuffer, inkReadFramebuffer);

    inkWriteFramebuffer->use();
    advectMaterial->pushUniform("x", inkReadFramebuffer->getFrameTexture());
    advectMaterial->pushUniform("dissipation", INK_LONGEVITY);
    screen->draw(advectMaterial);
    std::swap(inkWriteFramebuffer, inkReadFramebuffer);
    glPopDebugGroup();

    // Vorocity

    // Diffuse
    if (VISCOSITY > 0)
    {
        for (size_t i = 0; i < JACOBI_ITERATIONS_DIFFUSE; ++i)
        {
            velWriteFramebuffer->use();
            float centerFactor = DX * DX / (VISCOSITY * deltaTime);
            float stencilFactor = 1.f / (4 + centerFactor);
            diffuseMaterial->pushUniform("x", velReadFramebuffer->getFrameTexture());
            diffuseMaterial->pushUniform("b", velReadFramebuffer->getFrameTexture());
            diffuseMaterial->pushUniform("alpha", centerFactor);
            diffuseMaterial->pushUniform("rBeta", stencilFactor);
            screen->draw(diffuseMaterial);
            std::swap(velWriteFramebuffer, velReadFramebuffer);
        }
    }

    // Divergence
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "divergence step");
    tempWriteFramebuffer->use();
    divMaterial->pushUniform("map", velReadFramebuffer->getFrameTexture());
    screen->draw(divMaterial);
    std::swap(tempWriteFramebuffer, tempReadFramebuffer);

    boundaryMaterial->pushUniform("scale", 1);

    diffuseMaterial->pushUniform("alpha", -DX * DX);
    diffuseMaterial->pushUniform("rBeta", 0.25f);
    diffuseMaterial->pushUniform("b", tempReadFramebuffer->getFrameTexture());

    for (int i = 0; i < JACOBI_ITERATIONS_GRAD; ++i)
    {
        psiWriteFramebuffer->use();
        boundaryMaterial->pushUniform("x", psiReadFramebuffer->getFrameTexture());
        screen->draw(boundaryMaterial);
        std::swap(psiWriteFramebuffer, psiReadFramebuffer);

        psiWriteFramebuffer->use();
        diffuseMaterial->pushUniform("x", psiReadFramebuffer->getFrameTexture());
        screen->draw(diffuseMaterial);
        std::swap(psiWriteFramebuffer, psiReadFramebuffer);
    }

    velWriteFramebuffer->use();
    boundaryMaterial->pushUniform("x", velReadFramebuffer->getFrameTexture());
    boundaryMaterial->pushUniform("scale", -1);
    screen->draw(boundaryMaterial);
    std::swap(velWriteFramebuffer, velReadFramebuffer);

    velWriteFramebuffer->use();
    gradMaterial->pushUniform("mapP", psiReadFramebuffer->getFrameTexture());
    gradMaterial->pushUniform("mapW", velReadFramebuffer->getFrameTexture());
    screen->draw(gradMaterial);
    glPopDebugGroup();


    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "blit");
    framebuffer->use();
    glViewport(0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    screenMaterial->pushUniform("screenTexture", velReadFramebuffer->getFrameTexture());
    screen->draw(screenMaterial);
    glViewport(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    screenMaterial->pushUniform("screenTexture", inkReadFramebuffer->getFrameTexture());
    screen->draw(screenMaterial);
    glViewport(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    screenMaterial->pushUniform("screenTexture", psiReadFramebuffer->getFrameTexture());
    screen->draw(screenMaterial);
    glViewport(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    screenMaterial->pushUniform("screenTexture", tempReadFramebuffer->getFrameTexture());
    screen->draw(screenMaterial);
    glPopDebugGroup();

    framebuffer->flush();

    swapBuffers();
}

void close()
{
    delete advectShader;
    delete advectMaterial; 
    delete diffuseShader;
    delete diffuseMaterial; 
    delete impulseShader;
    delete impulseMaterial;
    delete boundaryShader;
    delete boundaryMaterial;
    delete screen;

    delete velWriteFramebuffer;
    delete psiWriteFramebuffer;
    delete inkWriteFramebuffer;
    delete vorWriteFramebuffer;

    delete velReadFramebuffer;
    delete psiReadFramebuffer;
    delete inkReadFramebuffer;
    delete vorReadFramebuffer;

    delete tempWriteFramebuffer;
    delete tempReadFramebuffer;

    SDL_GL_DeleteContext(gContext);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();

    delete screenMaterial;
    delete framebuffer;
}
