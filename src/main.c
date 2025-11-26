#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#include <stdio.h>

#include "player.h"
#include "world.h"

#define SCREEN_W 1980
#define SCREEN_H 1080
#define CELL_SIZE 50

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simple Player", 1920, 1080, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    // WORLD
    World world;
    if (!loadMap("maps/1.map", &world)) {printf("Unable to load map"); return 0;}

    // PLAYER
    Player player;
    initPlayer(&player, 50.0, 100.0);

    bool running = true;
    SDL_Event event;


    while (running) {
       
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

    
        const bool* keys = SDL_GetKeyboardState(NULL);

        const float rotationSpeed = 0.0005f;
        const float moveSpeed = 0.03f;
        
        if (keys[SDL_SCANCODE_Q]) 
        {
            rotatePlayer(&player, -rotationSpeed); 
        }
        if (keys[SDL_SCANCODE_E]) 
        {
            rotatePlayer(&player, rotationSpeed);  
        }


        if (keys[SDL_SCANCODE_D]) 
        {  
            movePlayer(&player, moveSpeed, 0);
        }
        if (keys[SDL_SCANCODE_A]) 
        { 
            movePlayer(&player, -moveSpeed, 0);
        }
        if (keys[SDL_SCANCODE_S]) 
        { 
            movePlayer(&player, 0, -moveSpeed);
        }
        if (keys[SDL_SCANCODE_W]) 
        {  
            movePlayer(&player, 0, moveSpeed);
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black Background
        SDL_RenderClear(renderer);

        
        float hw = 15.0f;
        float hh = 12.0f;

        float cx = player.x + hw;
        float cy = player.y + hh;

        SDL_Vertex vertices[4];

        // Top Left
        vertices[0].position.x = cx + (-hw)*cosf(player.angle) - (-hh)*sinf(player.angle);
        vertices[0].position.y = cy + (-hw)*sinf(player.angle) + (-hh)*cosf(player.angle);
        vertices[0].color.r = 255; vertices[0].color.g = 255; vertices[0].color.b = 255; vertices[0].color.a = 255;

        // Top Right
        vertices[1].position.x = cx + (hw)*cosf(player.angle) - (-hh)*sinf(player.angle);
        vertices[1].position.y = cy + (hw)*sinf(player.angle) + (-hh)*cosf(player.angle);
        vertices[1].color.r = 255; vertices[1].color.g = 255; vertices[1].color.b = 255; vertices[1].color.a = 255;

        // Bottom Right
        vertices[2].position.x = cx + (hw)*cosf(player.angle) - (hh)*sinf(player.angle);
        vertices[2].position.y = cy + (hw)*sinf(player.angle) + (hh)*cosf(player.angle);
        vertices[2].color.r = 255; vertices[2].color.g = 255; vertices[2].color.b = 255; vertices[2].color.a = 255;

        // Bottom Left
        vertices[3].position.x = cx + (-hw)*cosf(player.angle) - (hh)*sinf(player.angle);
        vertices[3].position.y = cy + (-hw)*sinf(player.angle) + (hh)*cosf(player.angle);
        vertices[3].color.r = 255; vertices[3].color.g = 255; vertices[3].color.b = 255; vertices[3].color.a = 255;
        
        // Draw 2 Triangles
        int indices[6] = {0, 1, 2, 0, 2, 3};
        SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);

        for (int y = 0; y < world.height; y++) {
            for (int x = 0; x < world.width; x++) {
                if (isWall(&world, x, y)) {
                    SDL_FRect rect = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white walls
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    // 4. Cleanup
    SDL_Quit();
    return 0;
}