#include "BasicRayMarch.hpp"


vec3 Mult(float a, vec3 b) //& means passed by reference
{
    vec3 c {0.0,0.0,0.0};
    c.x = a*b.x;
    c.y = a*b.y;
    c.z = a*b.z;
    
    return c;
}

vec3 AddV(vec3 a, vec3 b) //Add vectors
{
    vec3 c {0.0,0.0,0.0};
    c.x = a.x+b.x;
    c.y = a.y+b.y;
    c.z = a.z+b.z;
    
    return c;
}

vec3 AddS(float b, vec3 a) //Add scalar to vector
{
    vec3 c {0.0,0.0,0.0};
    c.x = a.x + b;
    c.y = a.y + b;
    c.z = a.z + b;
    
    return c;
}

float Dot(vec3 a, vec3 b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;   
}

vec3 Cross(vec3 a, vec3 b)
{
    vec3 c {0.0,0.0,0.0};
    c.x = a.y*b.z - a.z*b.y;
    c.y = a.z*b.x - a.x*b.z;
    c.z = a.x*b.y - a.y*b.x;
    
    return c;
}

vec3 Normalize(vec3 a)
{
    return Mult(1/sqrt(Dot(a,a)),a);
}

float Clamp(float num, float start, float end)
{
    return std::fmin(std::fmax(start,num),end);   
}

vec3 MultM(Mat3 M, vec3 V)
{
    vec3 c = {0.0,0.0,0.0};
    c.x = M.x_00*V.x + M.x_01*V.y +M.x_02*V.z;
    c.y = M.x_10*V.x + M.x_11*V.y +M.x_12*V.z;
    c.z = M.x_20*V.x + M.x_21*V.y +M.x_22*V.z;
    return c;
    
}
