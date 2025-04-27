#pragma once

#include<string>
#include<vector>
#include<map>
#include<functional>

#include<geGL/OpenGL.h>

std::string loadTxtFile     (std::string const&fileName);
void        error           (std::string const&name,std::string const&msg);
std::string shaderTypeToName(GLuint type);
GLuint      createShader    (GLuint type,std::string const&src);
GLuint      createProgram   (std::map<GLenum,GLuint>const&shaders);
bool        hasShader       (std::string const&txt,GLenum type);
bool        hasSubstring    (std::string const&txt,std::string w);
