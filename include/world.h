#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

#define MAX_MAP_W 250
#define MAX_MAP_H 250

typedef struct 
{
    int width;
    int height;
    int tiles[MAX_MAP_H][MAX_MAP_W];
} World;

bool generateMap(World* world, unsigned long seed, int width, int height);

bool isWall_int(World* world, int x, int y);
bool isWall_float(World* world, float x, float y, int gridSize);

#endif