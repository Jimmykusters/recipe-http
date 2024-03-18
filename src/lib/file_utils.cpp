#include "file_utils.h"


bool file_utils::writeToFile(const std::string &path, const std::string &value)
{
    std::ofstream file(path.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return false;
    }

    file << value;
    file.close();
    return true;
}

bool file_utils::appendToFile(const std::string &path, const std::string &value)
{
    std::ofstream file(path, std::ios::app); // Open the file in append mode
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return false;
    }

    file << value;
    file.close();
    return true;
}


void file_utils::creatFile(const std::string &path, const std::string &filename)
{
    std::string pathFile = path + filename;
    // Check if the file exists
    std::ifstream fileCheck(pathFile);
	
    if (!fileCheck.is_open()) {
        std::cout << "File does not exist. Creating a new file.\n";
        // Create the file if it doesn't exist
        std::ofstream createFile(pathFile);
        createFile.close();
    }
}

int file_utils::countLines(const std::string &path, const std::string &filename)
{
    std::string pathFile = path + filename;
    
    std::ifstream inputFile(pathFile);

    // Check if the file is successfully opened
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open the file.\n";
        return 1;
    }

    // Count the lines
    int lineCount = 0;
    std::string line;

    while (std::getline(inputFile, line)) {
        // Increment the line count for each line
        lineCount++;
    }

    // Close the file
    inputFile.close();
    return lineCount;
}