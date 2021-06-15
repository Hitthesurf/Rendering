#include "BasicRayMarch.hpp"

float Sphere(vec3 p, float r)
{
    return sqrt(Dot(p,p))-r;
}

float Torus(vec3 p, float r, float R) //r = radius of circle around the radius of circle R
{
    float p_xy = sqrt(p.x*p.x+p.y*p.y);
    return sqrt((p_xy-R)*(p_xy-R)+p.z*p.z)-r;
}