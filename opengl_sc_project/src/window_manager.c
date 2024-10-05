#include "window_manager.h"

void setup_window() {
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
        exit(1);
    }

    window = SDL_CreateWindow("SDL Animation",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    texture = 
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!texture) {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void update_window() {

    // Update the texture with the new pixel data
    SDL_UpdateTexture(texture, NULL, pixel_map, WINDOW_WIDTH * sizeof(unsigned int));

    // Render the texture
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}