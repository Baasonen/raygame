#ifndef PLAYER_H
#define PLAYER_H

typedef struct
{
    float x;
    float y;
    float angle;
    float speed;
    float fov;
    int raysPerSec;
} Player;

void initPlayer(Player* player, float starX, float startY);

void movePlayer(Player* player, float deltaX, float DeltaY);

void rotatePlayer(Player* player, float deltaAngle);

#endif