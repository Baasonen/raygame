#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#include <stdio.h>

#include "player.h"
#include "world.h"

#define SCREEN_W 1000
#define SCREEN_H 1000
#define CELL_SIZE 100

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simple Player", SCREEN_H, SCREEN_W, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    // WORLD
    World world;
    if (!loadMap("maps/1.map", &world)) {printf("Unable to load map"); return 0;}

    // PLAYER
    Player player;
    initPlayer(&player, 300.00, 250.0);
    float hw = 15.0f;
    float hh = 15.0f;

    bool running = true;
    SDL_Event event;


    while (running) {
       
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

    
        const bool* keys = SDL_GetKeyboardState(NULL);

        const float rotationSpeed = 0.0005f;
        const float moveSpeed = 0.03f;

        float moveX = 0;
        float moveY = 0;
        
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
            moveX += moveSpeed;
        }
        if (keys[SDL_SCANCODE_A]) 
        { 
            moveX -= moveSpeed;
        }
        if (keys[SDL_SCANCODE_W]) 
        { 
            moveY -= moveSpeed;
        }
        if (keys[SDL_SCANCODE_S]) 
        {  
            moveY += moveSpeed;
        }

        int currentX = (player.x) / CELL_SIZE;
        int currentY = (player.y) / CELL_SIZE;

        int newX = (player.x  + moveX) / CELL_SIZE;
        int newY = (player.y + moveY) / CELL_SIZE;

        //if (!isWall(&world, newX, player.y))
        //{
        //    movePlayer(&player, moveX, 0);
        //}
        //

        if (!isWall(&world, currentX, newY))
        {
            movePlayer(&player, 0, moveY);
        }
  
        if (!isWall(&world, newX, currentY))
        {   
            movePlayer(&player, moveX, 0);
        }

        //movePlayer(&player, moveX, 0);

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black Background
        SDL_RenderClear(renderer);

        float cx = player.x;
        float cy = player.y;

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