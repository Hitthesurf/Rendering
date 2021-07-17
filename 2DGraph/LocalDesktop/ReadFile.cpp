#include "Graph2D.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

//Read File Function
std::string ReadFile(std::string filename)
{
    std::string ShaderCode;
    std::ifstream ShaderFile;
    // ensure ifstream objects can throw exceptions:
    ShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        ShaderFile.open(filename);
        std::stringstream ShaderStream;
        // read file's buffer contents into streams
        ShaderStream << ShaderFile.rdbuf();		
        // close file handlers
        ShaderFile.close();
        
        // convert stream into string
        ShaderCode = ShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
	return ShaderCode;

}

std::string ReadFileAndJoin(std::string files[], int Elements)
{
	std::string FileText = "";
	for (int i=0; i<Elements; i++)
	{
		FileText += ReadFile(files[i]) + "\n";
	}
	return FileText;
	
}
