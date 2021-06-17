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

float ZoomCount = 1.0;

struct vec2 {
	float x;
	float y;
};

vec2 AddV(vec2 a, vec2 b) {
	vec2 a_add_b;
	a_add_b.x = a.x + b.x;
	a_add_b.y = a.y + b.y;
	return a_add_b;
};
	

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

vec2 processCamInput(GLFWwindow *window, float speed)
{
	vec2 Cam_Vel = {0.0,0.0};
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
		Cam_Vel.y += speed;
	}
	    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
		Cam_Vel.y -= speed;
	}
	    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
		Cam_Vel.x += speed;
	}
	    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
		Cam_Vel.x -= speed;
	}
	return Cam_Vel;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout<<width<<"x"<<height<<"\r";
} 

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ZoomCount -= yoffset;	
}

int main()
{
    
        /* Initialize the library */
    if (!glfwInit())
        return -1;
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Causes program to not run on my laptop
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  
    
    
    
    //Read the shaders from file 
    // 1. retrieve the vertex/fragment source code from filePath

    std::string fragmentCode;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        fShaderFile.open("FragmentShader.txt");
        std::stringstream fShaderStream;
        // read file's buffer contents into streams
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        fShaderFile.close();
        
        // convert stream into string
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* fragmentShaderSource = fragmentCode.c_str();
    
   
    
   
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
	vec2 CamPosValue = {0.0,0.0};
	unsigned int frames = 0;
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
		//Declare uniforms
        //float timeValue = glfwGetTime();
		float ZoomLevel = exp(0.4*ZoomCount);
		CamPosValue = AddV(CamPosValue,processCamInput(window,0.01*ZoomLevel));
		
        //int timeLocation = glGetUniformLocation(shaderProgram, "time");
		int CamPosLocation = glGetUniformLocation(shaderProgram, "CamPos");
		int ZoomLocation = glGetUniformLocation(shaderProgram, "Zoom");
		
        //glUniform1f(timeLocation,timeValue);
		glUniform2f(CamPosLocation, CamPosValue.x, CamPosValue.y);
        glUniform1f(ZoomLocation, ZoomLevel); //So zoom is always positive
        
		//If constant frame rate scroll will be consistent at any zoom level
		
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
		frames++;
    }
   
	std::cout<<"Frames: "<<frames<< " FPS: "<<(float)frames/glfwGetTime(); 
	glfwTerminate();
    return 0;
}