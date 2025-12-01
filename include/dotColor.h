#include <stdint.h>
#include <math.h>

void colorGradient(float dist, unsigned char* r, unsigned char* g, unsigned char* b)
{
    //dist /= 500.0f;

    if (dist < 0.0f) {dist = 0.0f;}
    if (dist > 1.0f) {dist = 1.0f;}

    float t = powf(dist, 2.0f);

    *r = (unsigned char)(t * 255.0f);
    *g = (unsigned char)((1.0f - dist) * 255.0f);
    *b = 0;
}