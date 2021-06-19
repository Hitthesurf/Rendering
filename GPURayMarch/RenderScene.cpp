#include "RenderScene.hpp"
#define GLFW_DLL //Only if dynamic lib //Dont forget to change depending on setup
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <cmath>

//Linker section, libs to include
// -lopengl32 -lglfw3 -lgdi32 -luser32 -lkernel32
//g++.exe "$(FILE_NAME)" -o $(NAME_PART) -lopengl32 -lglfw3 -lgdi32 -luser32 -lkernel32

float move_speed = 2.;
const float mouse_sen = 1.0;
float ZoomCount = 0.0;
//float fps = 55;


vec3 GetH(vec3 d)
{
    d = Normalize(d);
    vec3 H = {d.y, -d.x, 0};
    return Normalize(H);
}


vec3 processCamInput(GLFWwindow *window, float speed, vec3 CamDir)
{
	vec3 H = GetH(CamDir);
	vec3 V = Cross(H,CamDir);
	
	vec3 Cam_Vel = {0.0,0.0,0.0};
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
		Cam_Vel = Add(Cam_Vel,Mult(speed, CamDir));
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
		Cam_Vel = Add(Cam_Vel,Mult(-speed, CamDir));
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
		Cam_Vel = Add(Cam_Vel,Mult(speed,H));
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
		Cam_Vel = Add(Cam_Vel,Mult(-speed,H));
	}
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Cam_Vel = Add(Cam_Vel,Mult(speed,V));
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		Cam_Vel = Add(Cam_Vel,Mult(-speed,V));
	}
	
	return Cam_Vel;
}
	

//Should read in from files
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "out vec2 FragCoord;"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    "   FragCoord = vec2(aPos.x, aPos.y);"
    "}\n";

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {glfwSetWindowShouldClose(window, true);}
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ZoomCount -= (float)yoffset;	
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout<<width<<"x"<<height<<"\r";
} 

int main()
{
    
        /* Initialize the library */
    if (!glfwInit())
	{return -1;}
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scroll_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  
    

    
    //Read the shaders from file 
    // 1. retrieve the vertex/fragment source code from filePath
	std::string my_shader[] = {"Shaders/Setup.txt",
							   "Shaders/Materials.txt",
							   "Shaders/PrimitiveShapes.txt",
							   "Shaders/SceneDist.txt",
							   "Shaders/ShaderFragment.txt"};
							   
    std::string fragmentShaderSourceString = ReadFileAndJoin(my_shader, 5); //Cannot directly convert to c char
	//std::cout << fragmentShaderSourceString;
    const char* fragmentShaderSource = fragmentShaderSourceString.c_str();
    
   
    //We will now define vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    //Connect vertex shader to source code and compile
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    //Check if shader compiled
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //Connect fragment shader to source code
    unsigned int fragmentShader; //Stores the fragment shader at this location
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    //Check if fragment shader complied correctly
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //Now we must turn these shaders into a graphics pipeline
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    //Check for errors in the linker
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //Use the pipeline we created
    glUseProgram(shaderProgram);//Can also be called in render loop
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 
    

    
    //Now have Vertex Array Object to tell GL what the buffer has
     glViewport(0, 0, 800, 600);
    
    //Set up vertex data and buffers
    
    float vertices[] = {
        1.0f, -1.0f, // bottom right
         1.0f,  1.0f, // top right         
        -1.0f, -1.0f, // bottom left
        -1.0f,  1.0f, // top left

    };
     
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);  //Tell GL we want to generate 1 buffer object with ID VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Here we declare the type of buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Loads the data into the buffer
    
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); //Select VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);// Declare type and connect to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Store data
    
    //Now tell GL how to interpret vertex data in memory
    //Position data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    

    
    
    
// render loop
	vec3 CamPosValue = {0.0,0.0,1.0};
	vec3 CamDirValue = {1.0,0.0,0.0};
	CamDirValue = Normalize(CamDirValue);
	unsigned int frames = 0;
	float time = glfwGetTime();
	float timeEnd = time +0.05;
	float timeDiff = timeEnd - time;
    while(!glfwWindowShouldClose(window))
    {

		
		time = glfwGetTime();
		

		
		//Mouse Input
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		CamDirValue = vec3 {1.0,0.0,0.0};
		CamDirValue = Mult(R_y(ypos*mouse_sen/200.),CamDirValue);
		CamDirValue = Mult(R_z(-xpos*mouse_sen/200.),CamDirValue);
		
		// input
        processInput(window);
		processCamInput(window, move_speed, CamDirValue);

        // rendering commands here
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
		//Declare uniforms
        //float timeValue = glfwGetTime();
		CamPosValue = Add(CamPosValue, Mult(timeDiff, processCamInput(window,move_speed,CamDirValue)));
		
		
        //int timeLocation = glGetUniformLocation(shaderProgram, "time");
		int CamPosLocation = glGetUniformLocation(shaderProgram, "CamPos");
		int CamDirLocation = glGetUniformLocation(shaderProgram, "CamDir");
		int ZoomLocation = glGetUniformLocation(shaderProgram, "Zoom");
		
		
        //glUniform1f(timeLocation,timeValue);
        glUniform3f(CamPosLocation, CamPosValue.x, CamPosValue.y, CamPosValue.z);
		glUniform3f(CamDirLocation, CamDirValue.x, CamDirValue.y, CamDirValue.z);
		glUniform1f(ZoomLocation, exp(-ZoomCount));
		
		//If constant frame rate scroll will be consistent at any zoom level
		
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
		frames++;


		
		/*
		//We want a constant fps
		if (glfwGetTime()-time < 1/fps)
		{
			Sleep((1/fps - (glfwGetTime()-time))*1000.);
		}*/
		timeEnd = glfwGetTime(); // If runs for long long time could encouter errors as not enough bits in float
		timeDiff = timeEnd - time;
    }
   
	std::cout<<"Frames: "<<frames<< " FPS: "<<(float)frames/glfwGetTime(); 
	glfwTerminate();
    return 0;
}