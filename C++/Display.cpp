#include "BasicRayMarch.hpp"
const char bright_map[12] = {' ', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@'};

int DisplaySetup(int rows)
{
    // Set output mode to handle virtual terminal sequences
    for (int row = 0; row<rows; row++)
    {
        std::cout<<"\n";
    }
    
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return GetLastError();
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return GetLastError();
    }        
    return 0;
}

void Display(float Grid[], int stride, int rows)
{
    //stride gives size of first row of 2D grid
    std::cout << '\033' << "[" << rows <<"A"; //Go back to top
    
    std::string sDisplay = "";
    for (int row = 0; row < rows; row++){
        for (int collum = 0; collum < stride; collum++){
            float num = Grid[row*stride + collum];
            int index = (int)(12*num -0.001);
            sDisplay += bright_map[index];
        }
        sDisplay += "\n";
    }
    std::cout << sDisplay << std::flush;
    
    
    //maybe add sleep time
}


/*
int main()
{
    int Error = DisplaySetup();
    float Grid[20] =  {1.,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,1.,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1};
    float Grid1[20] = {0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,1.,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,1.};
    float Grid2[20] = {0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,1.,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,1.,0.9};
    float Grid3[20] = {0.7,0.6,0.5,0.4,0.3,0.2,0.1,1.,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,1.,0.9,0.8};
    
    Display(Grid, 10, 2);
    Sleep(500);
    Display(Grid1, 10, 2);
    Sleep(500);
    Display(Grid2, 10, 2);
    Sleep(500);
    Display(Grid3, 10, 2);
    Sleep(500);
    
    std::cout << "\n\n\n\n\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "\x1b[1A This is "<< i << "\r";
        //std::cout << "Hello Num " << i << "\n";
        Sleep(1000);
    }
    std::cout << "\n\n\n\n\n";

    return 0;
}
*/