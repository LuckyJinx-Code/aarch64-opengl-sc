#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "cglm/cglm.h"
#include "parameters.h"

void reset_modelmatrix();
void set_position(float posX, float posY);
void gen_rectangle(float w, float h, vec2 pos, float rad, float ang, float *vertices);
void add_to_buffer(GLuint *VBO, float *vertices, unsigned int size);
void set_color(vec4 new_color);
void draw(int VBO, int num_vertices, int mode);
char* read_file(FILE *fp);

#endif
