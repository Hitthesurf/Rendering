//#define GLFW_DLL //Only if dynamic lib
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <cmath>

//Linker section, libs to include
// -lopengl32 -lglfw3 -lgdi32 -luser32 -lkernel32
//g++.exe "$(FILE_NAME)" -o $(NAME_PART) -lopengl32 -lglfw3 -lgdi32 -luser32 -lkernel32 //Also ling the glad.c

//#define GLFW_DLL //Only if dynamic lib
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>

//Linker section, libs to include
// -lopengl32 -lglfw3 -lgdi32 -luser32 -lkernel32
//g++.exe "$(FILE_NAME)" -o $(NAME_PART) -lopengl32 -lglfw3 -lgdi32 -luser32 -lkernel32

//Should read in from files
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos; layout (location = 1) in vec3 aColor;\n"
    "out vec4 vertexColor;"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y-0.2, aPos.z, 1.0);\n"
    "   vertexColor = vec4(aPos.x, 0.0,0.0,1.0)+vec4(aColor,0.0);"
    "}\n";

const char *fragmentShaderSource = "#version 330 core \n"
"in vec4 vertexColor;"
"out vec4 FragColor;"
"uniform vec4 ourColor;"
"void main(){FragColor = vertexColor+ourColor;} ";

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {glfwSetWindowShouldClose(window, true);}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  
    
   
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
    
    //Now tell GL how to interpret vertex data in memory
    glVertexAttribPointer(0,3,GL_FLOAT,false,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0); 
    
    //Now have Vertex Array Object to tell GL what the buffer has
     glViewport(0, 0, 800, 600);
    
    //Set up vertex data and buffers
    
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0,0.0,0.0, // top right
         0.5f, -0.5f, 0.0f, 0.0,1.0,0.0, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0,0.0,1.0, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0,0.0,1.0 , // top left
         -1.0, 0.5, 0.0,    0.0,1.0,0.0,
         -0.5,0.0,0.0,      1.0,0.0,0.0,
    };
    
    /*
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    }; */
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
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
    //Position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    
    //Color data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); 
    
    
    //Use Element Buffer Objects to object data
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
// render loop
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        float timeValue = glfwGetTime();
        float blueValue = (sin(timeValue)/2.0)+0.5;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation,0.0f, 0.0f, blueValue,1.0f);
        
        //glUseProgram(shaderProgram);
        //glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}