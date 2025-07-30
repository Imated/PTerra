#pragma once
#include <filesystem>
#include <string>
#include <fstream>
#include <ios>
#include <string.h>
#include "GLFW/glfw3.h"
#include "renderer/Window.h"

#include "Logger.h"

// fix this frfr
class Utils {
public:
    static bool isPressed(Terra::Window *window, int key) {
        return glfwGetKey(window->getWindow(), key) == GLFW_PRESS;
    }
    static std::string readFile(const char* filename) {
        std::ifstream in(filename, std::ios::binary);
        if(in) {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(contents.data(), contents.size());
            in.close();
            return contents;
        }
        ASSERT(("Unable to open file with the name of '" + std::string(filename) + "'").c_str());
        return "";
    }

    static void writeFile(const char* filename, const char* contents, size_t size) {
        std::ofstream out(filename, std::ios::binary);
        if(out.is_open()) {
            out.write(contents, size);
        }
        else
            ASSERT(("Unable to open file with the name of '" + std::string(filename) + "'").c_str());
    }

    static bool fileExists(const char* filename) {
        INFO("Checking if file exists: %s", filename);
        return std::filesystem::exists(filename);
    }
};
