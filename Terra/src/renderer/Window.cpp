#include "Window.h"

namespace Terra {
    static void windowSizeCallback(GLFWwindow* window, int width, int height);
    static void errorCallback(int error, const char* description);

    Window::Window(int width, int height, const std::string& title)
    {
        INFO("Initializing...");

        glfwSetErrorCallback(errorCallback);

        if (glfwInit() == GLFW_FALSE)
        {
            ASSERT("Failed to initialize GLFW! Aborting...");
            return;
        }

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);
        if (!window)
        {
            ASSERT("Failed to create window! Aborting...");
            glfwTerminate();
            return;
        }

        DEBUG("Successfully Created Window.");

        glfwSetWindowSizeCallback(window, windowSizeCallback);

        DEBUG("Successfully Initialized GLFW.");

        glfwMakeContextCurrent(window);

        glfwGetFramebufferSize(window, &width, &height);
        params.width = width;
        params.height = height;
        DEBUG("Successfully Fetched Window Size of: %d, %d", width, height);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            ASSERT("Failed to initialize OpenGL! Aborting...");
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }
        DEBUG("Successfully Initialized OpenGL.");

        glViewport(0, 0, width, height);

        INFO("Initialized Successfully.");
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    static void windowSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static void errorCallback(int error, const char* description)
    {
        ERR("GLFW Error %d: %s", error, description);
    }

    GLFWwindow* Window::getWindow() const
    {
        return window;
    }
}