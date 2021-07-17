#include "Graph2D.hpp"
#include <cmath>
	

vec2 Add(vec2 a, vec2 b) {
	vec2 a_add_b;
	a_add_b.x = a.x + b.x;
	a_add_b.y = a.y + b.y;
	return a_add_b;
};

vec2 Add(float a, vec2 b) {
	vec2 a_add_b;
	a_add_b.x = a + b.x;
	a_add_b.y = a + b.y;
	return a_add_b;
};

vec2 Add(vec2 a, float b) {
	vec2 a_add_b;
	a_add_b.x = a.x + b;
	a_add_b.y = a.y + b;
	return a_add_b;
};

vec3 Add(vec3 a, vec3 b) //Add vectors
{
    vec3 c {0.0,0.0,0.0};
    c.x = a.x+b.x;
    c.y = a.y+b.y;
    c.z = a.z+b.z;
    
    return c;
}

vec3 Add(float b, vec3 a) //Add scalar to vector
{
    vec3 c {0.0,0.0,0.0};
    c.x = a.x + b;
    c.y = a.y + b;
    c.z = a.z + b;
    
    return c;
}

vec3 Add(vec3 a, float b) //Add scalar to vector
{
    vec3 c {0.0,0.0,0.0};
    c.x = a.x + b;
    c.y = a.y + b;
    c.z = a.z + b;
    
    return c;
}

vec3 Mult(vec3 b, float a)
{
	vec3 c;
	c.x = a*b.x;
	c.y = a*b.y;
	c.z = a*b.z;
	return c;	
}

vec3 Mult(float a, vec3 b){
	vec3 c;
	c.x = a*b.x;
	c.y = a*b.y;
	c.z = a*b.z;
	return c;	
}

vec2 Mult(vec2 b, float a)
{
	vec2 c;
	c.x = a*b.x;
	c.y = a*b.y;
	return c;	
}

vec2 Mult(float a, vec2 b){
	vec2 c;
	c.x = a*b.x;
	c.y = a*b.y;
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

vec3 Mult(Mat3 M, vec3 V)
{
    vec3 c = {0.0,0.0,0.0};
    c.x = M.x_00*V.x + M.x_01*V.y +M.x_02*V.z;
    c.y = M.x_10*V.x + M.x_11*V.y +M.x_12*V.z;
    c.z = M.x_20*V.x + M.x_21*V.y +M.x_22*V.z;
    return c;    
}

//Rotation matrix
Mat3 R_x(float a) //Around x_axis
{
	Mat3 c = {1.0,0.0,0.0,
			  0.0,(float)cos(a),-(float)sin(a),
			  0.0,(float)sin(a),(float)cos(a)};
	return c;	
}

Mat3 R_y(float a) //Around x_axis
{
	Mat3 c = {(float)cos(a),0.0,(float)sin(a),
						0.0,1.0,0.0,
			  -(float)sin(a),0.0,(float)cos(a)};
	return c;	
}

Mat3 R_z(float a) //Around x_axis
{
	Mat3 c = {(float)cos(a),-(float)sin(a),0.0,
			  (float)sin(a),(float)cos(a),0.0,
			  0.0,0.0,1.0};
	return c;	
}

