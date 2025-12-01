#include "world.h"
#include <stdio.h>
#include <stdlib.h>

bool loadMap(const char* filename, World* world)
{
    FILE* f = fopen(filename, "r");

    if (!f) {return false;}

    if (fscanf(f, "%d %d\n", &world->width, &world->height) != 2)
    {
        fclose(f);
        return false;
    }

    if (world->width > MAX_MAP_W || world->height > MAX_MAP_H)
    {
        fclose(f);
        return false;
    }

    for (int y = 0; y < world->height; y++)
    {
        for (int x = 0; x < world->width; x++)
        {
            int c = fgetc(f);
            if (c == '0') {world->tiles[y][x] = 0;}
            else if(c == '1') {world->tiles[y][x] = 1;}
            
            // Skip newline at end of row
            if (x == world->width - 1) {fgetc(f);}
        }
    }

    fclose(f);
    return true;
}

bool isWall(World* world, int x, int y)
{
    // Out of bounds
    if (x < 0 || x >= world->width || y < 0 || y >= world->height) {return true;}

    return world->tiles[y][x] != 0;
}