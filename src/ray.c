#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "ray.h"

#define STEP_SIZE 1

static float randomDirectionInFOV(float playerAngle, float fov)
{
    float fovRadians = fov * (3.14 / 180.0f);
    float offset = ((float)rand() / RAND_MAX - 0.5f) * fovRadians;
    return playerAngle + offset;
}

void checkRayCollision(Ray* ray, World* world, float startX, float startY, float startAngle)
{
    float dx = cosf(startAngle);
    float dy = sinf(startAngle);

    const float stepSize = 1.0f;
    const int maxDist = 2000;

    float dist = 0.0f;

    while (dist < maxDist)
    {
        float testX = startX + dx * dist;
        float testY = startY + dy * dist;

        int tileX = (int)(testX / 40);
        int tileY = (int)(testY / 40);

        if (isWall(world, tileX, tileY))
        {
            ray->collX = testX;
            ray->collY = testY;
            ray->dist = dist;
            return;
        }

        dist += stepSize;
    }

    ray->collX = 0.0f;
    ray->collY = 0.0f;
    ray->dist = 0.0f;
}

void shootRay(Ray* ray, World* world, Player* player)
{
    // Random angle
    float angle = randomDirectionInFOV(player->angle, player->fov);

    // Cast ray
    checkRayCollision(ray, world, player->x, player->y, angle);
}