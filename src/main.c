// Copyright (C) 2025 Henri Paasonen - GPLv2
// See LICENSE for details

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#include <stdio.h>

#include "player.h"
#include "world.h"
#include "ray.h"
#include "dotColor.h"

#define SCREEN_W 1000
#define SCREEN_H 2000
#define CELL_SIZE 40
#define RAY_BUFFER 10000
#define RAY_AMNT 2
const float MS_PER_FRAME = (1000 / 480);

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Raygame", SCREEN_H, SCREEN_W, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    // WORLD
    World world;
    if (!loadMap("maps/1.txtmap", &world)) {printf("Unable to load map"); return 0;}

    // PLAYER
    Player player;
    initPlayer(&player, 200.00, 250.0);
    float hw = 15.0f;
    float hh = 15.0f;

    Ray rays[RAY_BUFFER];
    int rayIndex = 0;

    bool running = true;
    SDL_Event event;

    long frameStart;
    int frameTime;
    long lastFrame = SDL_GetTicks();
    long lastTime = SDL_GetTicks();
    int frameCount = 0;


    while (running) {
        frameStart = SDL_GetTicks();

        float dt = (frameStart - lastFrame) / 1000.0f;
        lastFrame = frameStart;

        long currentTime = SDL_GetTicks();
        frameCount++;

        if (currentTime - lastTime >= 500)
        {
            float fps = frameCount * 500.0f / (currentTime - lastTime);
            printf("FPS: %.2f\n", fps);
            
            frameCount = 0;
            lastTime = currentTime;
        }
       
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

    
        const bool* keys = SDL_GetKeyboardState(NULL);

        const float rotationSpeed = 4.0f;
        const float moveSpeed = 200.0f;

        float moveX = 0;
        float moveY = 0;
        
        // Keyboard Inputs
        if (keys[SDL_SCANCODE_W]) {moveY -= moveSpeed * dt;}
        if (keys[SDL_SCANCODE_S]) {moveY += moveSpeed * dt;}
        if (keys[SDL_SCANCODE_A]) {moveX -= moveSpeed * dt;}
        if (keys[SDL_SCANCODE_D]) {moveX += moveSpeed * dt;}
            
        if (keys[SDL_SCANCODE_Q]) {rotatePlayer(&player, -rotationSpeed * dt);}
        if (keys[SDL_SCANCODE_E]) {rotatePlayer(&player,  rotationSpeed * dt);}


        int currentX = (player.x) / CELL_SIZE;
        int currentY = (player.y) / CELL_SIZE;

        int newX = (player.x  + moveX) / CELL_SIZE;
        int newY = (player.y + moveY) / CELL_SIZE;

        if (!isWall(&world, currentX, newY)) {movePlayer(&player, 0, moveY);}
  
        if (!isWall(&world, newX, currentY)) {movePlayer(&player, moveX, 0);}

        // Cast Rays
        for (int i = 0; i < RAY_AMNT; i++)
        {
            shootRay(&rays[rayIndex], &world, &player);
            rayIndex = (rayIndex + 1) % RAY_BUFFER;
        }


        // RENDER
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black Background
        SDL_RenderClear(renderer);

        // Draw Walls (For Debug)
        //SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); 
        //for (int y = 0; y < world.height; y++) {
        //    for (int x = 0; x < world.width; x++) {
        //        if (isWall(&world, x, y)) {
        //            // Use SDL_FRect for consistency with float coordinates
        //            SDL_FRect rect = { (float)x * CELL_SIZE, (float)y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
        //            SDL_RenderFillRect(renderer, &rect);
        //        }
        //    }
        //}
        //
        // Draw Ray Vectors
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 100); 
        for (int i = 0; i < RAY_AMNT; i++) 
        {
            int index = (rayIndex - 1 - i + RAY_BUFFER) % RAY_BUFFER;
            SDL_RenderLine(renderer, player.x, player.y, rays[index].collX, rays[index].collY);
        }
        // Draw Collision Points
        long now = SDL_GetTicks();

        unsigned char dotR, dotG, dotB;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
        for (int i = 0; i < RAY_BUFFER; i++) 
        {
            SDL_FRect rect = {rays[i].collX - 1.0f, rays[i].collY - 1.0f, 2.0f, 2.0f };
            float lifetime = (float)((rayIndex - i + RAY_BUFFER) % RAY_BUFFER) / RAY_BUFFER;
            colorGradient(lifetime, &dotR, &dotG, &dotB);
            SDL_SetRenderDrawColor(renderer, dotR, dotG, dotB, 255);
            SDL_RenderFillRect(renderer, &rect);
        }


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


        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        //if (MS_PER_FRAME > frameTime) {SDL_Delay((long)MS_PER_FRAME - frameTime);}
    }


    SDL_Quit();
    return 0;
}