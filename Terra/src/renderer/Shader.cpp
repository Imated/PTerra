#include "Shader.h"
#include "misc/Utility.h"

namespace Terra {
    Shader::Shader(const char* vertexFilename, const char* fragmentFilename)
    {
        shaderID = glCreateProgram();
        const auto vertexShader = createShader(vertexFilename, GL_VERTEX_SHADER);
        const auto fragmentShader = createShader(fragmentFilename, GL_FRAGMENT_SHADER);

        glAttachShader(shaderID, vertexShader);
        glAttachShader(shaderID, fragmentShader);
        glLinkProgram(shaderID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    GLuint Shader::createShader(const char* filename, GLenum type) const
    {
        const std::string source = Utils::readFile(filename);
        const char* shaderSource = source.c_str();

        GLint isCompiled = 0;
        const GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(!isCompiled)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());
            ERR(errorLog.data());
            ERR("Could not compile shader!");
        }
        return shader;
    }

    #pragma region Shader Attributes
    int Shader::getUniformLocation(const char* name) const
    {
        const int location = glGetUniformLocation(shaderID, name);
        if (location == -1)
        {
            std::string msg = "Could not find uniform variable with name of '" + std::string(name) + "'! Not loading shader...";
            const char* warningLog = msg.c_str();
            WARN(warningLog);
        }
        return location;
    }

    void Shader::setMatrix4x4(const char* name, const float* matrix) const
    {
        const int location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
    }

    void Shader::setVector4(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
    {
        const int location = getUniformLocation(name);
        glUniform4f(location, x, y, z, w);
    }

    void Shader::setVector3(const char* name, GLfloat x, GLfloat y, GLfloat z) const
    {
        const int location = getUniformLocation(name);
        glUniform3f(location, x, y, z);
    }

    void Shader::setVector2(const char* name, GLfloat x, GLfloat y) const
    {
        const int location = getUniformLocation(name);
        glUniform2f(location, x, y);
    }

    void Shader::setFloat(const char* name, float value) const
    {
        const int location = getUniformLocation(name);
        glUniform1f(location, value);
    }

    void Shader::setBool(const char* name, bool value) const
    {
        const int location = getUniformLocation(name);
        glUniform1i(location, value);
    }
    #pragma endregion

    void Shader::activate() const
    {
        glUseProgram(shaderID);
    }

    void Shader::del() const
    {
        glDeleteProgram(shaderID);
    }
}