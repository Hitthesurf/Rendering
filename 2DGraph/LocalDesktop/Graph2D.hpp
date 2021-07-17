#include <iostream>


struct vec2 {
	float x;
	float y;
};

struct vec3 {
	float x;
	float y;
	float z;
};

struct Mat3
{
    float x_00; float x_01; float x_02;
    float x_10; float x_11; float x_12;
    float x_20; float x_21; float x_22;   
};

//Read File Function
std::string ReadFile(std::string filename);
std::string ReadFileAndJoin(std::string files[], int Elements);


//Maths Functions we will need
vec2 Add(vec2 a, vec2 b);
vec2 Add(float a, vec2 b);
vec2 Add(vec2 a, float b);

vec2 Mult(vec2 b, float a);
vec2 Mult(float a, vec2 b);


vec3 Add(vec3 a, vec3 b);
vec3 Add(float b, vec3 a);
vec3 Add(vec3 a, float b);

vec3 Mult(vec3 b, float a);
vec3 Mult(float a, vec3 b);

vec3 Mult(Mat3 M, vec3 V);

float Dot(vec3 a, vec3 b);
vec3 Cross(vec3 a, vec3 b);
vec3 Normalize(vec3 a);
float Clamp(float num, float start, float end);

//Rotation Matrix
Mat3 R_x(float a); //Around x axis
Mat3 R_y(float a); //Around y axis
Mat3 R_z(float a); //Around z axis