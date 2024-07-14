#define PI 3.14159265359
#include "math.h"

typedef struct
{
    int x,y;
    float a;
}xya;

typedef struct
{
    int x,y;
}V;

typedef struct
{
    V A,B;
}LINE;

float rad(float x)
{
    return x*PI/180;
}

float COS(float x)
{
    return cos(rad(x));
}

float SIN(float x)
{
    return sin(rad(x));
}

