#ifndef RAY_H
#define RAY_H

#include "world.h"
#include "player.h"

typedef struct
{
    float collX;
    float collY;
    float dist;
} Ray;

void shootRay(Ray* ray, World* world, Player* player);

#endif