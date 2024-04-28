#pragma once

constexpr unsigned int SCREEN_WIDTH = 1280;
constexpr unsigned int SCREEN_HEIGHT = 720;
constexpr glm::vec2 SCREEN = glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);

constexpr unsigned int TEX_RES_X = 512;
constexpr unsigned int TEX_RES_Y = 512;
constexpr glm::vec2 DIM = glm::vec2(TEX_RES_X, TEX_RES_Y);
constexpr glm::vec2 INV_DIM = glm::vec2(1.f / TEX_RES_X, 1.f / TEX_RES_Y);
constexpr glm::vec2 INVERSE_GRID_SCALE = glm::vec2(DIM.x / SCREEN_WIDTH, DIM.y / SCREEN_HEIGHT);

constexpr glm::vec4 INK_COLOUR = glm::vec4(0.7f, 0.1f, 0.6f, 1.f);
constexpr float INK_LONGEVITY = 0.997;
constexpr float VELOCITY_DISSIPATION = 1.f;

constexpr float BRUSH_RADIUS = 0.1;
constexpr unsigned int JACOBI_ITERATIONS_DIFFUSE = 50;
constexpr unsigned int JACOBI_ITERATIONS_GRAD = 50;
constexpr unsigned int JACOBI_ITERATIONS_INK = 20;

constexpr float VISCOSITY = 0.f;
constexpr float DX = 1.f;
