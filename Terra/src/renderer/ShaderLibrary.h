#pragma once
#include <memory>
#include <unordered_map>
#include "Shader.h"

namespace Terra {
    enum shaderType
    {
        Vertex,
        Fragment,
        Geometry
    };

    class ShaderLibrary
    {
    public:
        static const std::shared_ptr<Shader>& get(const std::string& name);
        static const std::shared_ptr<Shader>& load(const std::string& name, const std::unordered_map<shaderType, std::string>& paths);
        static void free(const std::string& name);
        static void set(const std::string& name, const std::shared_ptr<Shader>& shader);

    private:
        static std::unordered_map<std::string, std::unordered_map<shaderType, std::string>> shaderPaths;
        static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    };
}
