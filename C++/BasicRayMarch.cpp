#include "BasicRayMarch.hpp"

float t_global = 0.0;
float t_sin_global = 0.0; // Do not want these function in SceneDF as very expensive
float t_cos_global = 0.0; // So treat them like uniform var for each frame
Mat3 t_transform_matrix = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

float SceneDF(vec3 pos, float t)
{
    //Note rotations that dont change for each pixel should be calulated only once
    // per frame, but if using t here it is calculate multiple times per pixel
    float d = pos.z; //Ensures there is a ground plane(x,y axis)
    vec3 Sphere_pos = {4.0,0.0,3.5};
    
    
    vec3 Torus_pos = {4.0,0.0,3.5};
    
    //Translated Positions
    Sphere_pos = AddV(pos,Mult(-1.,Sphere_pos));
    Torus_pos = AddV(pos,Mult(-1.,Torus_pos));
    
    Torus_pos = MultM(t_transform_matrix, Torus_pos);//Matrix after translation, rotation is done before
    Sphere_pos = MultM(t_transform_matrix, Sphere_pos);
    
    Sphere_pos.z += 2*t_sin_global;
    
    d = fmin(Sphere(Sphere_pos,0.8), d);
    d = fmin(d, Torus(Torus_pos,0.5,2.5));
    
    return d;
}

float RayMarch(vec3 ro, vec3 rd, float t) //ray origin and ray direction
{
    vec3 rp = ro; //Ray position
    float s = 0; //Distance travelled
    for (int step_num = 0; step_num < MAX_STEPS; step_num++)
    {
        float dist = SceneDF(rp, t);
        rp = AddV(rp, Mult(dist, rd)); //Gives us new position
        s += dist;
        if (s >= MAX_DIST || s <= MIN_STEP)
        {   break; }        
    }
    
    return s;
    
}

vec3 GetNormal(vec3 p, float t) //Gets normal of the surface
{
    vec3 GradF = {0.0,0.0,0.0};
    float F = SceneDF(p, t);
    GradF.x = SceneDF(AddV(p,Mult(epsilon,x_hat)),t);
    GradF.y = SceneDF(AddV(p,Mult(epsilon,y_hat)),t); 
    GradF.z = SceneDF(AddV(p,Mult(epsilon,z_hat)),t); 
    GradF = AddS(-F,GradF);
    return Normalize(GradF);
}

float GetLight(vec3 p, sLight Light, float t)
{
    vec3 toLight = AddV(Light.Pos,Mult(-1.,p));
    vec3 LDir = Normalize(toLight);
    vec3 Normal = GetNormal(p,t);//Surface normal
    float Light_Level = Clamp(Dot(LDir,Normal),0.,1.); //Needs to be in range 0 and 1
    

    //Calculate shadows
    if (SceneDF(p, t)<=MIN_STEP)
    {
        float d = RayMarch(AddV(p,Mult(2.*MIN_STEP,Normal)),LDir, t);
        if ((d*d)<Dot(toLight,toLight))
        {
           Light_Level *= 0.1;     
        }
         
    }
    return Light_Level;
}

vec3 GetH(vec3 d)
{
    d = Normalize(d);
    vec3 H = {d.y, -d.x, 0};
    return Normalize(H);
}

float mainImage(vec3 pixel, sCamera Camera, sLight Light, float t) //But pixel.z = 0.0
{
    vec3 uv = {0.0, 0.0, 0.0};
    uv.x = (pixel.x-0.5*Camera.Res_x)/Camera.Res_y;
    uv.y = (pixel.y-0.5*Camera.Res_y)/Camera.Res_y;
    
    vec3 H = GetH(Camera.Dir); //Get the horizontal part of screen
    vec3 V = Cross(H,Camera.Dir);
    
    vec3 ro = Camera.Pos;
    vec3 rd = Normalize(AddV(Mult(Camera.Zoom,Camera.Dir), AddV(Mult(uv.x, H) , Mult(uv.y, V))));
    
    float s = RayMarch(ro, rd, t);
    
    
    return GetLight(AddV(ro,Mult(s,rd)), Light, t);
    
    
}

void GetGrid(float Grid[], sCamera Camera, sLight Light, float t)
{
    int elements = Camera.Res_x*Camera.Res_y;
    
    for (int row = 0; row < Camera.Res_y; row++)
    {
     for (int col = 0; col < Camera.Res_x; col++)
     {
         int index = row*Camera.Res_x+col;
         int pixel_y = Camera.Res_y-row;
         Grid[index] = mainImage(vec3 {(float)col,(float)pixel_y,0.0}, Camera, Light, t);
     }
        
    }
    
    
}



int main()
{
    StopWatch My_Watch;
    My_Watch.Start();
    sCamera my_cam = {{-2.0,0.,6.0},{1.0,0.0,-0.5},150,42,.5};//pos, dir, res x, res y, zoom
    sLight my_light = {7.,4.,10.}; //pos
    
    int elements = my_cam.Res_x*my_cam.Res_y;
    float Grid[elements];
    
    DisplaySetup(my_cam.Res_y);
    
    for (int TimeCycle = 0; TimeCycle <100; TimeCycle++)
    {
        //Update camera light 
        float t = ((float)TimeCycle)/10.;
        t_global = t;
        t_sin_global = sin(t);
        t_cos_global = cos(t);
        t_transform_matrix.x_00 = t_cos_global;
        t_transform_matrix.x_02 = t_sin_global;
        t_transform_matrix.x_11 = 1.0;
        t_transform_matrix.x_20 = -t_sin_global;
        t_transform_matrix.x_22 = t_cos_global;
        //my_light.Pos.x = 3.*cos(t);
        //my_light.Pos.y = 3.*sin(t);
        //my_cam.Zoom = t/3.;
        t_sin_global = sin(2.*t);
        
        GetGrid(Grid, my_cam, my_light, t);
        Display(Grid, my_cam.Res_x, my_cam.Res_y);
    }
    My_Watch.Stop();
    std::cout << "Time: " <<My_Watch.Time();

    
    return 0;
}