#ifndef MAIN_H
#define MAIN_H

#include <CL/opencl.h>
#include <SDL2/SDL.h>
#include "cglm/cglm.h"
#include "parameters.h"
#include "utils.h"
#include "window_manager.h"

// RGB8 32 bits x fragment
u_int32_t pixel_map[WINDOW_WIDTH][WINDOW_HEIGHT];

GLuint program, framebuffer, colorbuffer, depthbuffer;

mat4 modelMatrix;
GLuint modelMatrixLoc;

//Attributes

GLuint vertexLoc;

//Uniforms

vec4 color;
GLuint colorLoc;

//VBOs

GLuint square_VBO;

//Window

SDL_Window* window;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture;

int main();

#endif