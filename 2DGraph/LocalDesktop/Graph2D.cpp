#include "Graph2D.hpp"
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


const int start_Res_x = 750;
const int start_Res_y = 750;

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {glfwSetWindowShouldClose(window, true);}
	
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
    
    GLFWwindow* window = glfwCreateWindow(start_Res_x, start_Res_y, "MyOpenGLScene", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  
    
    //Read the shaders from file 
    // 1. retrieve the vertex shader source code from filePath
	std::string my_vertex_shader = "Shaders/vertex_shader.txt";							   
    std::string vertexShaderSourceString = ReadFile(my_vertex_shader); //Cannot directly convert to c char
    const char* vertexShaderSource = vertexShaderSourceString.c_str();
    
    // 2. retrieve the fragment shader source code from filePath
    std::string my_frag_shader = "Shaders/fragment_shader.txt";
    std::string fragShaderSourceString = ReadFile(my_frag_shader);
    const char* fragShaderSource = fragShaderSourceString.c_str();
    
    int  success;
    char infoLog[512];
   
    //Check and compile the vertex shader
	unsigned int vertexShader; //Give location in memory
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEXSHADER::COMPILATION_FAILED\n" << infoLog << " 1 "<< std::endl;
    }
	
    //Check and compile the fragment shader
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragShader);
	
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENTSHADER::COMPILATION_FAILED\n" << infoLog << " 1 " << std::endl;
    }
    
    
    //Link and create the shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
	

    
    //Check for errors in the linker

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::SHADERPROGRAM::LINKER::COMPILATION_FAILED\n" << infoLog << " 1 " << std::endl;
    }

	glDeleteShader(vertexShader);
    glDeleteShader(fragShader); 
	
    glViewport(0, 0, start_Res_x, start_Res_y);
    
    //Now set up vertex data and buffers
    float vertices_x[] = {
		//Position, 
        -1.  , -0.98, -0.96, -0.94, -0.92, -0.9 , -0.88, -0.86, -0.84,
       -0.82, -0.8 , -0.78, -0.76, -0.74, -0.72, -0.7 , -0.68, -0.66,
       -0.64, -0.62, -0.6 , -0.58, -0.56, -0.54, -0.52, -0.5 , -0.48,
       -0.46, -0.44, -0.42, -0.4 , -0.38, -0.36, -0.34, -0.32, -0.3 ,
       -0.28, -0.26, -0.24, -0.22, -0.2 , -0.18, -0.16, -0.14, -0.12,
       -0.1 , -0.08, -0.06, -0.04, -0.02,  0.  ,  0.02,  0.04,  0.06,
        0.08,  0.1 ,  0.12,  0.14,  0.16,  0.18,  0.2 ,  0.22,  0.24,
        0.26,  0.28,  0.3 ,  0.32,  0.34,  0.36,  0.38,  0.4 ,  0.42,
        0.44,  0.46,  0.48,  0.5 ,  0.52,  0.54,  0.56,  0.58,  0.6 ,
        0.62,  0.64,  0.66,  0.68,  0.7 ,  0.72,  0.74,  0.76,  0.78,
        0.8 ,  0.82,  0.84,  0.86,  0.88,  0.9 ,  0.92,  0.94,  0.96,
        0.98,  1.  };
    
    
    unsigned int VBO; //Define memory location
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_x), vertices_x, GL_STATIC_DRAW);
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_x), vertices_x, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
    
    //Set line width
    glLineWidth(4.0);
    
    //Define Uniforms
    float time = glfwGetTime();
    
    //Render Loop
    
        while(!glfwWindowShouldClose(window))
    {

		//Uniform updates
        time = glfwGetTime();
        
        //Get Uniforms from shader
        int timeLocation = glGetUniformLocation(shaderProgram, "time");
        
        //Set Uniforms
        glUniform1f(timeLocation,time);        
        
        
		// input
        processInput(window);

        // rendering commands here
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
		
		//ShaderProgram
		glUseProgram(shaderProgram); // To use this shader program
		glBindVertexArray(VAO);

		glDrawArrays(GL_LINE_STRIP, 0, 101);
		


        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
   
	glfwTerminate();
    return 0;
    
}