#include "player.h"
#include "world.h"
#include <math.h>

#define PI 3.14159f

void initPlayer(Player* player, float startX, float startY)
{
    player->x = startX;
    player->y = startY;
    player->angle = 0;
    player->speed = 0;
    player->fov = 60.0f;
    player->raysPerSec = 2;
}

void movePlayer(Player* player, float deltaX, float deltaY)
{
    player->x += deltaX; //* sinf(player->angle);

    player->y += deltaY; // * sinf(player->angle + PI / 2);
}

void rotatePlayer(Player* player, float deltaAngle)
{
    player->angle += deltaAngle;
    if (player->angle > 2 * PI) {player->angle -= 2 * PI;}
    if (player->angle < 0) {player->angle += 2 * PI;}
}