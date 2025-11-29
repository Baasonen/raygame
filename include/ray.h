#include "world.h"

typedef struct
{
    float collX;
    float collY;
    float dist;
} Ray;

void checkRayCollision(Ray* ray, World* world, float startX, float startY, float startAngle, float fov);