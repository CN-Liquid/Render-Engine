#pragma once
#include "glad/glad.h"
#include <iostream>

#ifdef DEBUG 

    #define GLCALL(x) GLclearError();x;logCall(__FILE__,__LINE__)

#else

    #define GLCALL(x) x

#endif


static void GLclearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static void logCall(const char* file ,int line )
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL Error : " << error << " at " << file<< ":"<<line << std::endl;
    }
}