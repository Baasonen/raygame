#include <stdbool.h>
#include <math.h>

#include "ray.h"

#define STEP_SIZE 1

void checkRayCollision(Ray* ray, World* world, float startX, float startY, float startAngle, float fov)
{
    bool hasHit = false;
    int iteration = 1;

    while (!hasHit)
    {
        int nextX = (startX + cosf(iteration * STEP_SIZE)) / 100;
        int nextY = (startY + sinf(iteration * STEP_SIZE)) / 100;

        if (isWall(world, nextX, nextY))
        {
            hasHit = true;
            ray->collX = (startX + cosf(iteration * STEP_SIZE));
            ray->collY = (startY + sinf(iteration * STEP_SIZE));
            ray->dist = iteration * STEP_SIZE;
        }
        else
        {
            iteration++;
        }
    }

}   