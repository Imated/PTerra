#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <ios>
#include "Logger.h"


class Utils
{
public:
    static std::string readFile(const char* filename)
    {
        std::ifstream in(filename, std::ios::binary);
        if(in)
        {
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
};
