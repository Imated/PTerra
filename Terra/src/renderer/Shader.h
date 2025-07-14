#pragma once
#include "glad/glad.h"
#include "vector"

namespace Terra {
    class Shader
    {
    public:
        GLuint shaderID;
        Shader(const char* vertexFilename, const char* fragmentFilename);
        Shader(): shaderID(0) { }

        void setMatrix4x4(const char* name, const float* matrix) const;
        void setVector4(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
        void setVector3(const char* name, GLfloat x, GLfloat y, GLfloat z) const;
        void setVector2(const char* name, GLfloat x, GLfloat y) const;
        void setFloat(const char* name, float value) const;
        void setInt(const char* name, int value) const;
        void setBool(const char* name, bool value) const;

        void activate() const;
        void del() const;
    private:
        int getUniformLocation(const char* name) const;
        GLuint createShader(const char* filename, GLenum type) const;
    };
}
