#include "ShaderLibrary.h"
#include "misc/Utility.h"

namespace Terra {
    std::unordered_map<std::string, std::unordered_map<shaderType, std::string>> ShaderLibrary::shaderPaths;
    std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderLibrary::shaders;

    const std::shared_ptr<Shader>& ShaderLibrary::get(const std::string& name)
    {
        return shaders.at(name);
    }

    const std::shared_ptr<Shader>& ShaderLibrary::load(const std::string& name, const std::unordered_map<shaderType, std::string>& paths)
    {
        shaderPaths[name] = paths;
        set(name, std::make_unique<Shader>(Shader(paths.at(Vertex).c_str(), paths.at(Fragment).c_str())));
        return get(name);
    }

    void ShaderLibrary::free(const std::string& name)
    {
        shaderPaths.erase(name);
        shaders.erase(name);
    }


    void ShaderLibrary::set(const std::string& name, const std::shared_ptr<Shader>& shader)
    {
        shaders[name] = shader;
    }
}