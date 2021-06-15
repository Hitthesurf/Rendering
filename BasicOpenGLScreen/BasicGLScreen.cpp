//#define GLFW_DLL //Only if dynamic lib
#include <GLFW/glfw3.h>
#include <cmath>

//Linker section, libs to include
// -lopengl32 -lglfw3 -lgdi32 -luser32 -lkernel32
//g++.exe "$(FILE_NAME)" -o $(NAME_PART) -lopengl32 -lglfw3 -lgdi32 -luser32 -lkernel32

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    float counter = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5+sin(counter),-0.5);
        glVertex2f(0.0,0.5);
        glVertex2f(0.5,-0.5);
        glVertex2f(0.5,0.5);
        glVertex2f(0.7,0.5);
        glVertex2f(0.7,0.7);
        glEnd();
        counter += 0.1;
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}