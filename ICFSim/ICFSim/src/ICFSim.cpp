#include <iostream>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

#include "Constants.h"

#include "Framebuffer.h"

static SDL_GLContext gContext;

static SDL_Window* gWindow = nullptr;

Framebuffer* framebuffer = nullptr;

Uint32 lastUpdateTime = SDL_GetTicks();

Shader* fluidShader;
Material* fluidMaterial;
Model* fluid;

Shader* boundaryShader;
Material* boundaryMaterial;
Model* boundary;

bool init();
void draw(float deltaTime);
void close();

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
        }
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float deltaTime = (SDL_GetTicks() - lastUpdateTime) / 1000.f;
        draw(deltaTime);
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
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLErrorCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    framebuffer = new Framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, "screenTexture", new Material(new Shader("shaders/screen.vert.glsl", "shaders/screen.frag.glsl")));

    fluidShader = new Shader("shaders/fluid.vert.glsl", "shaders/fluid.frag.glsl");
    fluidMaterial = new Material(fluidShader);
    fluid = new Model(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, fluidMaterial);

    boundaryShader = new Shader("shaders/boundary.vert.glsl", "shaders/boundary.frag.glsl");
    boundaryMaterial = new Material(boundaryShader);
    boundary = new Model(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 100, boundaryMaterial);

    return true;
}

void draw(float deltaTime)
{
    framebuffer->use();

    boundary->draw();
    fluid->draw();

    framebuffer->flush();
}

void close()
{
    delete fluidShader;
    delete fluidMaterial;
    delete fluid;
    delete boundaryShader;
    delete boundaryMaterial;
    delete boundary;

    SDL_GL_DeleteContext(gContext);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();

    delete framebuffer;
}
