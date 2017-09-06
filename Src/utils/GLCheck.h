#ifndef __GLCHECK_H__
#define __GLCHECK_H__

#include <sstream>
#include <unordered_map>

#include <GL/glew.h>

#include "utils/Logger.h"


static void checkGLErrors(const char* func, const char* file, int line)
{
    static std::unordered_map<GLenum, const char*> Tags = {
        { GL_INVALID_ENUM, "Invalid Enum" },
        { GL_INVALID_FRAMEBUFFER_OPERATION, "Invalid Framebuffer Operation" },
        { GL_INVALID_OPERATION, "Invalid Operation" },
        { GL_INVALID_VALUE, "Invalid Value" },
        { GL_OUT_OF_MEMORY, "Out of Memory" },
        { GL_STACK_OVERFLOW, "Stack Overflow" },
        { GL_STACK_UNDERFLOW, "Stack Underflow" }
    };

    GLenum err = GL_NO_ERROR;
    while ( (err = glGetError()) != GL_NO_ERROR )
    {
        std::stringstream ss;
        ss << "OpenGL error in "
           << func
           << " ("
           << file
           << ", line "
           << line
           << ") : "
           << Tags[err];
        Logger::Error(ss.str());
    }
}


#ifdef _DEBUG
    #define GLCHECK(func) \
        func; \
        checkGLErrors(#func, __FILE__, __LINE__);
#else
    #define GLCHECK(func) func;
#endif // _DEBUG


#endif // __GLCHECK_H__
