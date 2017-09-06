#include "viewer/ShaderProgram.h"

#include <fstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "utils/Logger.h"


static bool _ReadShader(const std::string& fileName, std::string& oSource)
{
    std::ifstream stream(fileName);
    if (!stream.is_open())
    {
        Logger::Error("Failed to open file " + fileName);
        return false;
    }

    std::string line;
    while (getline(stream, line))
        oSource += line + "\n";

    stream.close();
    return true;
}


static bool _CompileShader(GLuint shader, const std::string& shaderFile)
{
    assert(shader != 0);
    Logger::Info("Compiling shader " + shaderFile);

    std::string source;
    if (!_ReadShader(shaderFile, source))
        return false;

    // Compile
    const GLchar* pSource = static_cast<const GLchar*>(source.c_str());
    GLCHECK(glShaderSource(shader, 1, &pSource, nullptr));
    GLCHECK(glCompileShader(shader));

    // Check errors
    GLint status = 0;
    GLCHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE)
    {
        GLint logLength = 0;
        GLCHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength));

        GLchar* log = new GLchar[logLength + 1];
        if (log)
        {
            GLCHECK(glGetShaderInfoLog(shader, logLength, &logLength, log));

            Logger::Error("Compilation of shader " + shaderFile + " failed (see below for details)");
            std::cerr << log << std::endl;

            delete[] log;
        }

        return false;
    }

    return true;
}


ShaderProgram::ShaderProgram()
    : m_pid(0)
    , m_vFile("")
    , m_fFile("")
{}

ShaderProgram::ShaderProgram(const std::string& vFile, const std::string& fFile)
    : m_pid(0)
    , m_vFile(vFile)
    , m_fFile(fFile)
{
    compileAndLink();
}

ShaderProgram::~ShaderProgram()
{
    clear();
}


bool ShaderProgram::compileAndLink()
{
    bool compileOk = true;
    bool linkOk = true;

    // First delete old program if necessary
    if (m_pid)
        clear();

    GLCHECK(m_pid = glCreateProgram());
    if (m_pid == 0)
        Logger::Fatal("Failed to create OpenGL shader program");

    GLCHECK(GLuint vid = glCreateShader(GL_VERTEX_SHADER));
    if (vid == 0)
        Logger::Fatal("Failed to create OpenGL vertex shader");
    GLCHECK(GLuint fid = glCreateShader(GL_FRAGMENT_SHADER));
    if (fid == 0)
        Logger::Fatal("Failed to create OpenGL fragment shader");

    compileOk = _CompileShader(vid, m_vFile) && _CompileShader(fid, m_fFile);
    if (compileOk)
    {
        Logger::Info("Linking shader program");

        // Link
        GLCHECK(glAttachShader(m_pid, vid));
        GLCHECK(glAttachShader(m_pid, fid));
        GLCHECK(glLinkProgram(m_pid));

        // Check errors
        GLint status = 0;
        GLCHECK(glGetProgramiv(m_pid, GL_LINK_STATUS, &status));
        if (status == GL_FALSE)
        {
            linkOk = false;

            GLint logLength = 0;
            GLCHECK(glGetProgramiv(m_pid, GL_INFO_LOG_LENGTH, &logLength));

            GLchar* log = new GLchar[logLength + 1];
            if (log)
            {
                GLCHECK(glGetProgramInfoLog(m_pid, logLength, &logLength, log));

                Logger::Error("Program link failed (see below for details)");
                std::cerr << log << std::endl;

                delete[] log;
            }
        }

        GLCHECK(glDetachShader(m_pid, vid));
        GLCHECK(glDetachShader(m_pid, fid));
    }

    GLCHECK(glDeleteShader(vid));
    GLCHECK(glDeleteShader(fid));

    return compileOk && linkOk;
}

GLint ShaderProgram::getAttribute(const std::string& name)
{
    auto it = m_attributes.find(name);
    if (it != m_attributes.end())
        return it->second;

    GLCHECK(GLint loc = glGetAttribLocation(m_pid, name.c_str()));
    if (loc == -1)
        Logger::Debug("Attribute " + name + " does not exist");
    else
        m_attributes.insert({ name, loc });

    return loc;
}

GLint ShaderProgram::getUniform(const std::string& name)
{
    auto it = m_uniforms.find(name);
    if (it != m_uniforms.end())
        return it->second;

    GLCHECK(GLint loc = glGetUniformLocation(m_pid, name.c_str()));
    if (loc == -1)
        Logger::Debug("Uniform " + name + " does not exist");
    else
        m_uniforms.insert({ name, loc });

    return loc;
}

void ShaderProgram::setUniform(const std::string& name, bool b)
{
    GLCHECK(glUniform1i(getUniform(name), b ? 1 : 0));
}

void ShaderProgram::setUniform(const std::string& name, int i)
{
    GLCHECK(glUniform1i(getUniform(name), i));
}

void ShaderProgram::setUniform(const std::string& name, float f)
{
    GLCHECK(glUniform1f(getUniform(name), f));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec2& vec)
{
    GLCHECK(glUniform2fv(getUniform(name), 1, glm::value_ptr(vec)));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& vec)
{
    GLCHECK(glUniform3fv(getUniform(name), 1, glm::value_ptr(vec)));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec4& vec)
{
    GLCHECK(glUniform4fv(getUniform(name), 1, glm::value_ptr(vec)));
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat3& mat)
{
    GLCHECK(glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(mat)));
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& mat)
{
    GLCHECK(glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(mat)));
}


void ShaderProgram::clear()
{
    if (m_pid)
    {
        GLCHECK(glDeleteProgram(m_pid));
        m_pid = 0;
    }
    m_attributes.clear();
    m_uniforms.clear();
}
