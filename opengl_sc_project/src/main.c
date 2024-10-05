#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main() {

    //Setting up window

    setup_window();

    // Set Up Frame Context
    glGenFramebuffers(1, &framebuffer);
    glGenRenderbuffers(1, &colorbuffer);
    glGenRenderbuffers(1, &depthbuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, WINDOW_WIDTH, WINDOW_HEIGHT);

    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, WINDOW_WIDTH, WINDOW_HEIGHT);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorbuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    program = glCreateProgram();

    FILE *kernel_file = fopen("kernel.ocl","r");
    
    fseek(kernel_file , 0 , SEEK_END);
    long fsize = ftell(kernel_file);
    rewind(kernel_file);
    
    void* data = malloc(fsize);
    size_t size = fread(data, 1, fsize, kernel_file);
    
    fclose(kernel_file);
    
    glProgramBinary(program, 0, data, size);

    //LINK_PROGRAM(program, "kernel");
    glUseProgram(program);
    
    // Set Up Vertex Attributes
    vertexLoc  = glGetAttribLocation(program, "vertex");

    float square_vertices[18];

    float rect_width = 100.0f;
    float rect_height = 60.0f;

    gen_rectangle(rect_width, rect_height, (vec2) {0.0f,0.0f}, 0, 0, square_vertices);
    add_to_buffer(&square_VBO, square_vertices, sizeof(square_vertices));

    colorLoc = glGetUniformLocation(program,"color");
    modelMatrixLoc = glGetUniformLocation(program,"modelMatrix");

    glReadnPixels(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, 
            GL_RGBA, GL_UNSIGNED_BYTE, WINDOW_WIDTH*WINDOW_HEIGHT*4, pixel_map);
    
    /*for (int i = 0; i < WINDOW_HEIGHT; i++) {
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            printf("[%X]", 
                pixel_map[j][i]);
        };
        printf("\n");
    }*/
    

    //print_ppm("output.ppm", WINDOW_WIDTH, WINDOW_HEIGHT, (uint8_t*) pixel_map);
    
    unsigned int frameDelay = 1000 / 60;
    unsigned int frameStart;
    int frameTime;

    int quit = 0;
    SDL_Event e;

    float posX = 0.0f;

    int curr_time = 0;
    int num_frames = 0;

    // Main loop
    while (!quit) {
        frameStart = SDL_GetTicks();  // Start the frame timer

        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        reset_modelmatrix();
        set_position(posX, 0.0f);
        set_color((vec4) {255.0f, 255.0f, 0.0f, 255.0f});

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw(square_VBO, 6, GL_TRIANGLES);

        glFinish();

        glReadnPixels(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, 
            GL_RGBA, GL_UNSIGNED_BYTE, WINDOW_WIDTH*WINDOW_HEIGHT*4, pixel_map);

        update_window();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        posX += (WINDOW_WIDTH+rect_width)*frameTime/(3*1000);
        if (posX > (WINDOW_WIDTH+rect_width)/2)
            posX -= (WINDOW_WIDTH+rect_width);
        
        num_frames++;
        int new_time = curr_time + frameTime;
        if (new_time/1000 != curr_time/1000) {
            printf("FPS: %d\n", num_frames);
            num_frames = 0;
        }
        curr_time = new_time;
    }

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}