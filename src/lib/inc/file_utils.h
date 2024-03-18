#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace file_utils
{
    bool writeToFile(const std::string &path, const std::string &value);
    bool appendToFile(const std::string &path, const std::string &value);
    void creatFile(const std::string &path, const std::string &filename);
    int countLines(const std::string &path, const std::string &filename);
}
