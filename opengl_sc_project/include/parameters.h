#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLSC2/glsc2.h>
#include "cglm/cglm.h"
#include "SDL2/SDL.h"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 250

extern u_int32_t pixel_map[WINDOW_WIDTH][WINDOW_HEIGHT];

extern mat4 modelMatrix;
extern GLuint modelMatrixLoc;

extern GLuint program, framebuffer, colorbuffer, depthbuffer;

//Attributes

#define POS_ATTR 0

extern GLuint vertexLoc;

//Uniforms

extern vec4 color;
extern GLuint colorLoc;

//VBOs

extern GLuint square_VBO;

//Window

extern SDL_Window* window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;

#endif
