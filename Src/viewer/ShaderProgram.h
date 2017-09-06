#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#include <memory>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "utils/GLCheck.h"


class ShaderProgram
{
public:
    ShaderProgram();
    ShaderProgram(const std::string& vFile, const std::string& fFile);

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ~ShaderProgram();

    void setVertexFile(const std::string& vFile) { m_vFile = vFile; }
    void setFragmentFile(const std::string& fFile) { m_fFile = fFile; }

    bool compileAndLink();

    GLint getAttribute(const std::string& name);
    GLint getUniform(const std::string& name);

    void setUniform(const std::string& name, bool b);
    void setUniform(const std::string& name, int i);
    void setUniform(const std::string& name, float f);
    void setUniform(const std::string& name, const glm::vec2& vec);
    void setUniform(const std::string& name, const glm::vec3& vec);
    void setUniform(const std::string& name, const glm::vec4& vec);
    void setUniform(const std::string& name, const glm::mat3& mat);
    void setUniform(const std::string& name, const glm::mat4& mat);

    void start() const { GLCHECK(glUseProgram(m_pid)); }
    static void Stop() { GLCHECK(glUseProgram(0)); }

private:
    GLuint m_pid;

    std::string m_vFile;
    std::string m_fFile;

    std::unordered_map<std::string, GLuint> m_attributes;
    std::unordered_map<std::string, GLuint> m_uniforms;

    void clear();
};

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;

#endif // __SHADERPROGRAM_HPP__

