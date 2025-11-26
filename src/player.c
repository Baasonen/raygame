#include "player.h"
#include <math.h>

#define PI 3.14159f

void initPlayer(Player* player, float startX, float startY)
{
    player->x = startX;
    player->y = startY;
    player->angle = 0;
    player->speed = 0;
}

void movePlayer(Player* player, float forward, float sideways)
{
    player->x += forward * cosf(player->angle);
    player->y += forward * sinf(player->angle);

    player->x += sideways * cosf(player->angle + PI / 2);
    player->y += sideways * sinf(player->angle + PI / 2);
}

void rotatePlayer(Player* player, float deltaAngle)
{
    player->angle += deltaAngle;
    if (player->angle > 2 * PI) {player->angle -= 2 * PI;}
    if (player->angle < 0) {player->angle += 2 * PI;}
}