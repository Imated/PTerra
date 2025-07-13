#pragma once
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include <iostream>
#include "misc/Logger.h"

class Window {
public:

    Window(int width, int height, const std::string& title = "Game");
    ~Window();
    
    GLFWwindow* getWindow() const;
    glm::vec2 getSize() { return { params.width, params.height }; }

private:

    GLFWwindow* window = nullptr;
    GLFWglproc context = nullptr;

    struct WindowParams
    {
        int width;
        int height;
    } params;
};
