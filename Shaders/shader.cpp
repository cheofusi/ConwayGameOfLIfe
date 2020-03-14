#include <GL/glew.h>
#include <iostream>
#include <fstream>

#include "shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path){
    std::string vertShaderStr = readFile(vertex_path);
    std::string fragShaderStr = readFile(fragment_path);
    
    const char* vertexShaderSource = vertShaderStr.c_str();
    const char* fragmentShaderSource = fragShaderStr.c_str();

    //vertexshader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    char infoLog[512]; //first 512 bytes of the compile log.
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR--SHADER--VERTEX--COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //fragmentshader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR--SHADER--FRAGMENT--COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    Program = glCreateProgram();
    glAttachShader(Program, vertexShader);
    glAttachShader(Program, fragmentShader);
    glLinkProgram(Program);
    // check for linking errors
    glGetProgramiv(Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(Program, 512, NULL, infoLog);
        std::cout << "ERROR--SHADER--PROGRAM--LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>

std::string Shader::readFile(const char *filePath) const{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);  

    if(!fileStream.is_open()){
        std::cout<< "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    //content.append("\0");
    fileStream.close();
    return content;
}

void Shader::use(){
    glUseProgram(Program);
}

void Shader::setFloat(const std::string& name, float value0, float value1, float value2, float value3) const 
{ 
    glUniform4f(glGetUniformLocation(Program, name.c_str()), value0, value1, value2, value3); 
} 