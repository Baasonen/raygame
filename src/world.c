#include "world.h"
#include <stdio.h>
#include <stdlib.h>

static float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

static float fade(float t)
{
    return t * t * t * (t * (t* 6 -15) +10);
}

static float noise2D(int x, int y, unsigned long seed)
{
    long s = seed;
    s ^= x * 374761393u;
    s ^= y * 668265263u;
    s = (s ^ (s >> 13)) * 1274126177u;
    return (float)(s & 0xFFFF) / 65535.0f;
}

static float perlin2D(float x, float y, unsigned long seed)
{
    int xi = (int)x;
    int yi = (int)y;

    float xf = x - xi;
    float yf = y - yi;

    float n00 = noise2D(xi, yi, seed);
    float n10 = noise2D(xi + 1, yi, seed);
    float n01 = noise2D(xi, yi + 1, seed);
    float n11 = noise2D(xi + 1, yi + 1, seed);

    float u = fade(xf);
    float v = fade(yf);

    float nx0 = lerp(n00, n10, u);
    float nx1 = lerp(n01, n11, u);

    return lerp(nx0, nx1, v);
}

bool generateMap(World* world, unsigned long seed, int width, int height)
{
    if (width > MAX_MAP_W || height > MAX_MAP_H) {return false;}

    world->width = width;
    world->height = height;

    float freq = 0.2f;  // lower -> larger feature
    float threshold = 0.5f;

    printf("StartGen");

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float n = perlin2D(x * freq, y * freq, seed);

            world->tiles[y][x] = (n < threshold) ? 1 : 0;
        }
    }

    return true;
}

bool isWall_int(World* world, int x, int y)
{
    // Out of bounds
    if (x < 0 || x >= world->width || y < 0 || y >= world->height) {return true;}

    return world->tiles[y][x] != 0;
}

bool isWall_float(World* world, float x, float y, int grid_size)
{
    int xi = (int)(x / grid_size);
    int yi = (int)(y / grid_size);

    // Out of bounds
    if (xi < 0 || xi >= world->width || yi < 0 || yi >= world->height) {return true;}

    return world->tiles[yi][xi] != 0;
}