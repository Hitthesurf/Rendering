#include <cmath>
#include <iostream>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

//StopWatch
#include <ctime>
#include <ratio>
#include <chrono>

const int MAX_STEPS = 100;
const float MAX_DIST = 100.;
const float MIN_STEP = 0.01;
const float epsilon = MIN_STEP*0.2;


struct vec3
{
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

const vec3 x_hat = {1.0,0.0,0.0};
const vec3 y_hat = {0.0,1.0,0.0};
const vec3 z_hat = {0.0,0.0,1.0};

struct sCamera
{
    vec3 Pos;
    vec3 Dir;
    int Res_x;
    int Res_y;
    float Zoom;
};

struct sLight
{
    vec3 Pos;
    
};

//BasicRayMarch Functions
vec3 GetNormal(vec3 pos);
float RayMarch(vec3 ro, vec3 rd, float t);
float SceneDF(vec3 p, float t);




//DisplayFunction
int DisplaySetup(int rows);
void Display(float Grid[], int stride, int rows);



//PrimitiveObjectectFunctions
float Sphere(vec3 p, float r);
float Torus(vec3 p, float r, float R);



//Maths Functions for vectors


vec3 Cross(vec3 a, vec3 b);
float Dot(vec3 a, vec3 b);
vec3 Mult(float a, vec3 b);
vec3 AddV(vec3 a, vec3 b);
vec3 AddS(float b, vec3 a);
vec3 Normalize(vec3 a);
float Clamp(float num, float start, float end);
vec3 MultM(Mat3 M, vec3 V);


//StopWatch
class StopWatch {
private:
 std::chrono::high_resolution_clock::time_point t1;
 std::chrono::high_resolution_clock::time_point t2;

public:
 void Start();
 void Stop();
 float Time();
};