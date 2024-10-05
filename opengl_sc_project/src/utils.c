#include "utils.h"

void reset_modelmatrix() {
    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
}

void set_position(float posX, float posY) {
    glm_translate(modelMatrix,(vec3){posX, posY, 0.0});
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
}

void gen_rectangle(float w, float h, vec2 pos, float rad, float ang, float *vertices) {
    float centerX = pos[0] + rad*cos(ang);
    float centerY = pos[1] + rad*sin(ang);

    vec3 ul_corner = {
        centerX - cos(ang)*w/2 - sin(ang)*h/2,
        centerY + cos(ang)*h/2 - sin(ang)*w/2,
        0.0
    };

    vec3 ur_corner = {
        centerX + cos(ang)*w/2 - sin(ang)*h/2,
        centerY + cos(ang)*h/2 + sin(ang)*w/2,
        0.0
    };

    vec3 lr_corner = {
        centerX + cos(ang)*w/2 + sin(ang)*h/2,
        centerY - cos(ang)*h/2 + sin(ang)*w/2,
        0.0
    };

    vec3 ll_corner = {
        centerX - cos(ang)*w/2 + sin(ang)*h/2,
        centerY - cos(ang)*h/2 -sin(ang)*w/2,
        0.0
    };

    float gen_vertices[] = {
        ll_corner[0], ll_corner[1], 0.0f,
        ul_corner[0], ul_corner[1], 0.0f,
        ur_corner[0], ur_corner[1], 0.0f,
        ll_corner[0], ll_corner[1], 0.0f,
        ur_corner[0], ur_corner[1], 0.0f,
        lr_corner[0], lr_corner[1], 0.0f
    };
    memcpy(vertices, gen_vertices, sizeof(gen_vertices));
}

void set_color(vec4 new_color) {
    glUniform4fv(colorLoc,1, new_color);
}

void add_to_buffer(GLuint *VBO, float *vertices, unsigned int size) {
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void draw(int VBO, int num_vertices, int mode) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(mode, 0, num_vertices);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#define INIT_ALLOC 1024
#define MIN_SPACE   256
#define MAX_EXCESS  256

char* read_file(FILE *fp) {
    size_t offset = 0;
    size_t bufsiz  = INIT_ALLOC;
    char *buffer = malloc(bufsiz);
    if (buffer == NULL)
        return NULL;
    while (fgets(buffer + offset, bufsiz - offset, fp) != NULL)
    {
        /* Assumes data does not contain null bytes */
        /* Generic problem using fgets() */
        size_t newlen = strlen(buffer + offset);
        offset += newlen;
        if (bufsiz - offset < MIN_SPACE)
        {
            size_t new_size = bufsiz * 2;
            char  *new_data = realloc(buffer, new_size);
            if (new_data == NULL)
            {
                free(buffer);
                return NULL;
            }
            bufsiz = new_size;
            buffer = new_data;
        }
    }
    if (bufsiz - offset > MAX_EXCESS)
        buffer = realloc(buffer, offset + 1);
    return buffer;
}